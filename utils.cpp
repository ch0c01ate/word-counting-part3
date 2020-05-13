#include <sstream>
#include <fstream>
#include <archive.h>
#include <archive_entry.h>
#include "utils.h"
#include <boost/filesystem/path.hpp>

void
getConfig(std::map<std::string, std::string> &config, int &indexingThreadNum, int &mergeThreadNum, int &maxQueueSize,
          const char *file) {
    std::ifstream configFile(file);

    if (!configFile.good())
        throw std::ios_base::failure("Wrong path for config file by a!");

    std::string line, key, value;

    while (getline(configFile, line)) {

        if (line.empty() || line[0] == '#' || line[0] == ' ')
            continue;

        std::istringstream lineStream(line);
        if (std::getline(lineStream, key, '=')) {
            if (std::getline(lineStream, value)) {
                if (key == "max_queue_size")
                    maxQueueSize = std::stoi(value);
                else if (key == "indexing_thread_num")
                    indexingThreadNum = std::stoi(value);
                else if (key == "merge_thread_num")
                    mergeThreadNum = std::stoi(value);
                else
                    config.insert(std::make_pair(key, value));
            }
        }
    }

    std::ifstream readfile(config["infile"]);
    if (!readfile.good())
        throw std::ios_base::failure("Wrong path for readfile!");

    std::ofstream writeByN(config["out_by_n"]);
    if (!writeByN.good())
        throw std::ios_base::failure("Wrong path for writing by n!");

    std::ofstream writeByA(config["out_by_a"]);
    if (!writeByA.good())
        throw std::ios_base::failure("Wrong path for writing by a!");

    if (indexingThreadNum < 1)
        throw std::invalid_argument("Wrong number of indexing threads!");

    if (mergeThreadNum < 1)
        throw std::invalid_argument("Wrong number of merging threads!");

    if (maxQueueSize < 2)
        throw std::invalid_argument("Wrong max queue size!");
}


void readIso(const std::string &file, tbb::flow::queue_node<std::string> node) {
    struct archive *a;
    struct archive *a2;
    struct archive_entry *entry;
    struct archive_entry *entry2;
    int r;

    a = archive_read_new();

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, file.c_str(), 10240);
    if (r != ARCHIVE_OK)
        exit(1);

    std::string dest;

    int i =0;

    std::vector<std::string> wordsVector;

    std::string text;

    while (
            archive_read_next_header(a, &entry
            ) == ARCHIVE_OK) {

        if (i > 0){
            break;
        }
        boost::filesystem::path entryPath = boost::filesystem::path(archive_entry_pathname(entry));
        if (entryPath.extension() != ".zip" && entryPath.extension() != ".ZIP")
            continue;

        auto size = archive_entry_size(entry);
        dest = std::string(size, 0);
        r = archive_read_data(a, &dest[0], dest.size());


        archive_read_data_skip(a);
        a2 = archive_read_new();
        archive_read_support_filter_all(a2);
        archive_read_support_format_all(a2);

        r = archive_read_open_memory(a2, dest.c_str(), dest.size());

        if (r != ARCHIVE_OK) {
            archive_free(a2);
            continue;

        }

        // This file crashes the program
        if (std::string(archive_entry_pathname(entry)).find("51753-0") != -1) {
            archive_free(a2);
            continue;
        }

        while (archive_read_next_header(a2, &entry2) == ARCHIVE_OK) {

            boost::filesystem::path entryPath2 = boost::filesystem::path(archive_entry_pathname(entry2));
            if (entryPath2.extension() != ".txt" && entryPath2.extension() != ".TXT")
                continue;



            auto size2 = archive_entry_size(entry2);
            if (size2 > 10000000)
                continue;
            ++i;
            text = std::string(size2, 0);
            r = archive_read_data(a2, &text[0], text.size());

            if (r == ARCHIVE_FATAL || r == ARCHIVE_EOF || r == ARCHIVE_RETRY) {
                continue;
            }

            if (!text.empty()) {
//                bool isPushed = false;
//                while(!isPushed)
                  std::cout << node.try_put(text);
            }
            std::cout<<"TUT";
        }

        archive_free(a2);
    }


    archive_free(a);
//    bool isPushed = false;
//    while(!isPushed)
    node.try_put(std::string{});
}




