//
// Created by Bernadzikovskiy Slavik on 5/12/20.
//

#ifndef WORD_COUNTING_PART3_UTILS_H
#define WORD_COUNTING_PART3_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <boost/locale.hpp>

template <class T>
class concurrent_que{
    std::deque<T> que_m;
    mutable std::mutex m_m;
    std::condition_variable cv_m;
    std::condition_variable cv_push;
    int limit = INT_MAX;
    bool finished = false;

public:
    concurrent_que()= default;

    explicit concurrent_que(int setLimit): limit(setLimit){}

    void pop(T &d){
        std::unique_lock<std::mutex> lg{m_m};
        cv_m.wait(lg, [this](){ return que_m.size() > 0; });
        d = que_m.back();
        que_m.pop_back();

        cv_push.notify_one();
    }

    void push(const T v){

        std::unique_lock<std::mutex> lg{m_m};

        cv_push.wait(lg, [this](){ return que_m.size() < limit; });
        que_m.push_front(std::move(v));
        cv_m.notify_one();

    }

    void pushHard(const T v){
        std::unique_lock<std::mutex> lg{m_m};
        que_m.push_front(v);
        cv_m.notify_one();
    }

    void finish(){
        std::unique_lock<std::mutex> lg{m_m};
        finished = true;
    }

    bool finishedPushing(){
        std::unique_lock<std::mutex> lg{m_m};
        return finished;
    }


    size_t size() const { std::lock_guard<std::mutex> lg{m_m}; return que_m.size(); }
};

void getConfig(std::map<std::string, std::string> &config, int &indexingThreadNum, int &mergeThreadNum, int &maxQueueSize, const char* file);
void readIso(const std::string &file, concurrent_que<std::string> &q);

#endif //WORD_COUNTING_PART3_UTILS_H
