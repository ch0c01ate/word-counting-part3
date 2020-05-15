//
// Created by heeve on 01.04.20.
//

#include <atomic>
#include "merge.h"


void merge(myMap &left, myMap &right) {
    for (const auto &itr: right) {
        if (!itr.first.empty())
            left[itr.first] += itr.second;
    }
//    myMap().swap(right);
}

//void mergeMaps(concurrent_que<myMap> &q) {
//
//
//    if (q.size() <= 1 && q.finishedPushing()) {
//        return;
//    }
//    myMap m1;
//
//    q.pop(m1);
//
//
//    if (q.size() == 0 && q.finishedPushing()) {
//        q.pushHard(std::move(m1));
//
//        mergeMaps(q);
//    }
//
//
//    myMap m2;
//    q.pop(m2);
//
//    q.pushHard(std::move(merge(m1, m2)));
//
//    myMap().swap(m1);
//
//    mergeMaps(q);
//
//}

