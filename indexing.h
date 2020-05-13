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

myMap create_words_map(std::string &def_words_vector, std::locale& loc);

void create_result(const myMap &words_map, const std::string &out_path, std::map<std::string, std::string> &);

//bool index(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue);

//void parallelIndexing(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue, std::locale& loc);

#endif //WORD_COUNTING_INDEXING_H
