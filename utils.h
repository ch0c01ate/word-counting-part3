#ifndef WORD_COUNTING_UTILS_H
#define WORD_COUNTING_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <boost/locale/generator.hpp>
#include "merge.h"
#include "tbb/flow_graph.h"
#include "tbb/concurrent_hash_map.h"

//void readIso(const std::string &file, tbb::flow::function_node<std::string, tbb::concurrent_hash_map<std::string, int>> &node);
void readIso(const std::string &file, tbb::flow::function_node<std::string *> &node);

void getConfig(std::map<std::string, std::string> &config, int &indexingThreadNum, int &mergeThreadNum, int &maxQueueSize, const char* file);
//void readIso(const std::string &file, concurrent_que<std::string>& q);

#endif //WORD_COUNTING_UTILS_H
