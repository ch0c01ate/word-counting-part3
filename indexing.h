//
// Created by heeve on 05.04.20.
//

#ifndef WORD_COUNTING_INDEXING_H
#define WORD_COUNTING_INDEXING_H

#include "merge.h"
#include "utils.h"
#include <map>
#include <iterator>
#include <fstream>
#include <algorithm>
#include "tbb/flow_graph.h"
#include "tbb/concurrent_unordered_map.h"

void create_words_map(std::shared_ptr<std::string>&str, std::locale& loc,  tbb::flow::function_node<std::shared_ptr<tbb::concurrent_unordered_map<std::string, int>>> merger);

//void create_result(const myMap &words_map, const std::string &out_path, std::map<std::string, std::string> &);

bool index(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue);

void parallelIndexing(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue, std::locale& loc);

#endif //WORD_COUNTING_INDEXING_H
