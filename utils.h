#ifndef WORD_COUNTING_UTILS_H
#define WORD_COUNTING_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <boost/locale/generator.hpp>
#include "merge.h"


void getConfig(std::map<std::string, std::string> &config, int &indexingThreadNum, int &mergeThreadNum, int &maxQueueSize, const char* file);
void readIso(const std::string &file, concurrent_que<std::string>& q);

#endif //WORD_COUNTING_UTILS_H
