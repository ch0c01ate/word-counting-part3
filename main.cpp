#include <chrono>
#include <atomic>
#include "utils.h"
#include "indexing.h"
#include <boost/locale.hpp>
#include <utility>
#include "tbb/concurrent_queue.h"

std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

using concUnMap = tbb::concurrent_unordered_map<std::string, int>;
using concUnMapPtr = std::shared_ptr<concUnMap>;

template<class D>
long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

void
merge(concUnMapPtr &left, const concUnMapPtr &right) {
    for (const auto &itr: *right) {
        if (!itr.first.empty())
            (*left)[itr.first] += itr.second;
    }
    (*right).clear();
}

int main(int argc, char *argv[]) {

    std::map<std::string, std::string> config;
    const char *defaultConfigName = "./config.dat";
    int threadNum = 0;
    int mergeThreadNum = 0;
    int maxQueueSize = 0;


    if (argc == 2)
        getConfig(config, threadNum, mergeThreadNum, maxQueueSize, argv[1]);
    else if (argc == 1)
        getConfig(config, threadNum, mergeThreadNum, maxQueueSize, defaultConfigName);
    else
        throw std::invalid_argument("Wrong number of arguments!");

    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);

    tbb::flow::graph g;

    tbb::flow::limiter_node<std::shared_ptr<std::string>> indexingLimiter(g, maxQueueSize);
    tbb::flow::queue_node<concUnMapPtr> mergingQueueNode(g);
    tbb::concurrent_queue<concUnMapPtr> waitForMergeQueue;

    tbb::flow::function_node<concUnMapPtr, tbb::flow::continue_msg> merger
    (g,tbb::flow::unlimited,[&](concUnMapPtr left) {
//            std::cout << "Merge\n";
            concUnMapPtr right;
            if (waitForMergeQueue.empty()) {
                if (mergingQueueNode.try_get(right)) {
                    merge(left, right);
                    mergingQueueNode.try_put(left);
                } else {
                    waitForMergeQueue.push(std::move(left));
                }
            } else {
                waitForMergeQueue.try_pop(right);
                merge(left, right);
                mergingQueueNode.try_put(left);
            }
        });


    tbb::flow::function_node<std::shared_ptr<std::string>> indexer(g, tbb::flow::unlimited,
                                                    [&](std::shared_ptr<std::string> str) {
                                                        indexingLimiter.decrement.try_put(tbb::flow::continue_msg());
                                                        create_words_map(str, loc, mergingQueueNode);
                                                    });

    tbb::flow::function_node<std::string> reader(g, tbb::flow::serial, [&](const std::string &path) {
        return readIso(path, indexingLimiter);
    });

    tbb::flow::make_edge(mergingQueueNode, merger);
    tbb::flow::make_edge(indexingLimiter, indexer);
    reader.try_put(config["infile"]);

    g.wait_for_all();


    concUnMapPtr globalMap{};
    waitForMergeQueue.try_pop(globalMap);


    create_result(globalMap, std::ref(config["out_by_a"]), std::ref(config));
    create_result(globalMap, std::ref(config["out_by_n"]), std::ref(config));

    return 0;
}


