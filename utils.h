#ifndef WORD_COUNTING_UTILS_H
#define WORD_COUNTING_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <boost/locale/generator.hpp>
#include "merge.h"
#include "tbb/flow_graph.h"


void getConfig(std::map<std::string, std::string> &config, int &indexingThreadNum, int &mergeThreadNum, int &maxQueueSize, const char* file);
void readIso(const std::string &file, tbb::flow::function_node<std::string, tbb::flow::continue_msg> node);

#endif //WORD_COUNTING_UTILS_H
