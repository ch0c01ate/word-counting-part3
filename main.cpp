#include <chrono>
#include <atomic>
#include "utils.h"
#include "indexing.h"
#include <boost/locale.hpp>



std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int main(int argc, char *argv[]) {

    std::map<std::string, std::string> config;
    const char *defaultConfigName = "../config.dat";
    int threadNum = 0;
    int mergeThreadNum = 0;
    int maxQueueSize = 0;


    if (argc == 2)
        getConfig(config, threadNum, mergeThreadNum, maxQueueSize, argv[1]);
    else if (argc == 1)
        getConfig(config, threadNum, mergeThreadNum, maxQueueSize, defaultConfigName);
    else
        throw std::invalid_argument("Wrong number of arguments!");


    concurrent_que<std::string> wordsQueue(maxQueueSize);
    concurrent_que<myMap> mapsQueue(maxQueueSize);

    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);


    std::vector<std::thread> wordMapsThreads;

    auto start = get_current_time_fenced();

    for (int i = 0; i < threadNum; ++i) {
        wordMapsThreads.emplace_back(parallelIndexing, std::ref(wordsQueue), std::ref(mapsQueue), std::ref(loc));
    }

    std::vector<std::thread> mergeThreads;

    for (long i = 0; i < mergeThreadNum; ++i) {
        mergeThreads.emplace_back(mergeMaps, std::ref(mapsQueue));
    }

    readIso(config["infile"], wordsQueue);


    for (auto &thr: wordMapsThreads) {
        thr.join();
    }

    mapsQueue.finish();
    mapsQueue.push(std::move(myMap{}));


    for (auto &t: mergeThreads) {
        t.join();
    }

    auto end = get_current_time_fenced();



    myMap wordsMap;
    mapsQueue.pop(wordsMap);


    auto stopIndexing = get_current_time_fenced();

    std::cout << "Time in us: " << to_us(end - start) << std::endl;

    std::cout << wordsMap.size() << " unique words  \n";

    unsigned long counter = 0;
    for (auto &itr: wordsMap) {
        counter += itr.second;
    }
    std::cout << counter << " words in total \n";

    std::thread resByName(create_result, std::ref(wordsMap), std::ref(config["out_by_a"]), std::ref(config));
    std::thread resByNun(create_result, std::ref(wordsMap), std::ref(config["out_by_n"]), std::ref(config));
    resByName.join();
    resByNun.join();


    return 0;
}


