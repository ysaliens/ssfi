#include <iostream>
#include <string>
#include <atomic>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/program_options.hpp>
#include <boost/asio/post.hpp>

#include "worker.h"

// Recursively scans a directory looking for .txt files and adds them to worker pool
void index(const boost::filesystem::path &path, boost::asio::thread_pool &pool, 
    std::vector<std::unordered_map<std::string, int>> &maps, std::atomic<int>& threadIDs){

    boost::filesystem::recursive_directory_iterator file{path};   
    while (file != boost::filesystem::recursive_directory_iterator{}){
        if (file->path().extension() == ".txt" && !boost::filesystem::is_directory(file->path())){
            std::string filename = file->path().string();
            boost::asio::post(pool, [&, filename](){ process(filename, threadIDs, maps); });
        }
        *file++;
    }
}

// Merge hash maps, sort, print results
void print(std::vector<std::unordered_map<std::string, int>> &maps, int words){
    // Join all word count hash maps, using map 0 as main map
    for (size_t i=1; i<maps.size(); i++){
        for (auto item : maps[i]){
            maps[0][item.first] = maps[0][item.first] + maps[i][item.first];
        }
    }

    // Convert map 0 to vector, then sort based on values (word frequencies)
    std::vector<std::pair<std::string, int>> vec;
    std::copy(maps[0].begin(), maps[0].end(), std::back_inserter<std::vector<std::pair<std::string, int>>>(vec));
    std::sort(vec.begin(), vec.end(), [](std::pair<std::string, int>& a, std::pair<std::string, int>& b){
        return a.second > b.second;
    });

    // Print top words
    for (int i=0; i<words; i++){
        std::cout << vec[i].first << "  " << vec[i].second << std::endl;
    }
}

int main(int argc, char *argv[]){
    int threads;
    int words;
    std::string dir;

    // Parse arguments
    namespace opt = boost::program_options;
    opt::options_description description("SSFI Options:");
    description.add_options()
        ("help,h",      "Help message")
        ("path,p", opt::value<std::string>(&dir),      
                        "Directory to index")
        ("threads,t", opt::value<int>(&threads)->default_value(3),
                        "Number of worker threads")
        ("words,w", opt::value<int>(&words)->default_value(10),
                        "Top N words to print");
    opt::variables_map variablesMap;
    try {
        opt::store(opt::command_line_parser(argc, argv).options(description).run(), variablesMap);
        if (variablesMap.count("help")){
            std::cout << description << std::endl;
            exit(0);
        }
        opt::notify(variablesMap);
    } catch (opt::error &e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(-1);
    }

    // Validate inputs
    if (threads <= 0){
        std::cerr << "ERROR: Number of worker threads MUST be greater than 0."<< std::endl;
        exit(-1);
    }
    if (words <= 0){
        std::cerr << "ERROR: Number of top words MUST be greater than 0."<< std::endl;
        exit(-1);
    }
    boost::filesystem::path path(dir);
    if (!boost::filesystem::exists(path) || !boost::filesystem::is_directory(path)){
        std::cerr << "ERROR: Please provide a valid directory."<< std::endl;
        exit(-1);
    }

    // Create a worker thread pool
    boost::asio::thread_pool pool(threads);

    // Each worker thread stores word counts in an unordered hash table/map for speed
    // Maps persist per thread lifetime for efficiency, no locks are required
    std::vector<std::unordered_map<std::string, int>> maps;
    maps.resize(threads);

    // Index path recursively, add matching files to worker queue pool
    std::atomic<int> threadIDs(0);
    index(path, pool, maps, threadIDs);

    pool.join();

    print(maps, words);

    return 0;
}
