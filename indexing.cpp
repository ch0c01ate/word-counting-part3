//
// Created by heeve on 05.04.20.
//

#include <zconf.h>
#include <boost/locale.hpp>
#include "indexing.h"
#include "utils.h"


template<typename key_map, typename val_map>
std::pair<val_map, key_map> swap_pair_items(const std::pair<key_map, val_map> &p) {
    return std::pair<val_map, key_map>(p.second, p.first);
}

template<typename key_map, typename val_map>
std::multimap<val_map, key_map> swap_map_items(const std::unordered_map<key_map, val_map> &src) {
    std::multimap<val_map, key_map> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   swap_pair_items<key_map, val_map>);
    return dst;
}

void print_to_file(std::ofstream &out_file, const std::string &first_arg, const std::string &second_arg) {
    int max_length = 20;
    out_file << first_arg;
    for (int i = first_arg.size();
         i < ((first_arg.size() > max_length) ? ((int) first_arg.size() + 1) : max_length); ++i)
        out_file << " ";
    out_file << ":";
    for (int i = second_arg.size(); i < max_length; ++i)
        out_file << " ";
    out_file << second_arg << std::endl;
}

void
create_result(const myMap &words_map, const std::string &out_path, std::map<std::string, std::string> &config_map) {
    std::ofstream out_file;
    out_file.open(out_path, std::ios_base::out | std::ios_base::trunc);
    if (!out_file.is_open()) {
        std::cerr << "Error opening " << out_path << " file" << std::endl;
        return;
    }

    if (out_path == config_map["out_by_a"]) {
        std::map<std::string, int> ordered_words_map(words_map.begin(), words_map.end());
        for (auto &word_data : ordered_words_map) {
            print_to_file(out_file, word_data.first, std::to_string(word_data.second));
        }
    } else if (out_path == config_map["out_by_n"]) {
        std::multimap<int, std::string> dst = swap_map_items(words_map);
        for (auto word_data = dst.rbegin(); word_data != dst.rend(); ++word_data) {
            print_to_file(out_file, word_data->second, std::to_string(word_data->first));
        }
    }
    out_file.close();
}


// Indexing functions

myMap create_words_map(std::string &str, std::locale& loc) {
    myMap words_map;

    str = boost::locale::normalize(str, boost::locale::norm_nfd);
    str = boost::locale::fold_case(str);

    boost::locale::boundary::ssegment_index resultMap(boost::locale::boundary::word, str.begin(),
                                                      str.end(), loc);


    resultMap.rule(boost::locale::boundary::word_any);

    for (boost::locale::boundary::ssegment_index::iterator it = resultMap.begin(), e = resultMap.end();
         it != e; ++it) {
        if (!it->empty())
            ++words_map[*it];
    }

    if (!words_map.empty())
        return words_map;

    return myMap {};

}


//bool index(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue, std::locale& loc) {
//    std::string str;
//    wordsQueue.pop(str);
//
//    if (str.empty()) {
//        if (!wordsQueue.finishedPushing()) {
//            wordsQueue.finish();
//        }
//        wordsQueue.push(std::move(str));
//        return false;
//    }
//
//    create_words_map(str, mapsQueue, loc);
//
//    std::string().swap(str);
//
//    return true;
//
//}

//void parallelIndexing(concurrent_que<std::string> &wordsQueue, concurrent_que<myMap> &mapsQueue, std::locale& loc){
//    while(index(wordsQueue, mapsQueue, loc)){
//
//    }
//}