#include <string>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <algorithm>
#include <fstream>
#include <regex>
#include <atomic>

#include "worker.h"

void count(std::string path, std::unordered_map<std::string, int> &wordCounts){
    
    std::ifstream infile(path); 
    if (!infile.good()) return;

    // Use alpha-numeric regex for words
    static const std::regex pattern("[[:alnum:]]+");
    static const std::regex_iterator<std::string::iterator> iter;

    std::string line;
    getline(infile, line);
    while (infile.good()){
        std::regex_iterator<std::string::iterator> regexMatch(line.begin(), line.end(), pattern);
        while (regexMatch != iter){
            std::string word = regexMatch->str();
            transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Update word counts, or insert if word has never been seen
            if (wordCounts[word] >= 1) ++wordCounts[word];
            else wordCounts[word] = 1;

            ++regexMatch;
        }
        getline(infile, line);
    }
}

void process(std::string path, std::atomic<int>& id, std::vector<std::unordered_map<std::string, int>> &maps){
    // Get thread ID to know which map to access
    thread_local int myid = -1;
    if (myid == -1) myid = id.fetch_add(1);
    //std::cout << "ID:" << myid << std::endl;
    
    count(path, maps[myid]);
}
