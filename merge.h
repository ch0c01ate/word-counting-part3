//
// Created by heeve on 01.04.20.
//

#ifndef COUNT_WORDS_1_MERGE_H
#define COUNT_WORDS_1_MERGE_H

#include <iostream>
#include <vector>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <climits>


typedef std::unordered_map<std::string, int> myMap;


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
        d = std::move(que_m.back());
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
        que_m.push_front(std::move(v));
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

//void mergeMaps(concurrent_que<myMap> &q);
void merge(myMap &left, myMap &right);

#endif //COUNT_WORDS_1_MERGE_H
