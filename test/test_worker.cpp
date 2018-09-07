#include <unordered_map>
#include <string>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <vector>

#include "../src/worker.h"

int main (){
    std::cout << "Testing word counting in a file" << std::endl;
    std::string path = "files/test1.txt";
    std::unordered_map<std::string, int> map;

    count(path, map);

    assert(map["ten"] == 10);
    assert(map["five"] == 5);
    assert(map["bad"] == 0);
    assert(map["notThere"] == 0);
    std::cout << "Word count test complete" << std::endl << std::endl;


    std::cout << "Testing worker process function" << std::endl;
    std::string path2 = "files/test1.txt";
    std::vector<std::unordered_map<std::string, int>> maps;
    maps.resize(2);
    std::atomic<int> threadIDs(0);

    process(path2, threadIDs, maps);

    assert(maps[0]["ten"] == 10);
    assert(maps[0]["five"] == 5);
    assert(maps[0]["bad"] == 0);
    assert(maps[0]["notThere"] == 0);
    std::cout << "Process function test complete" << std::endl << std::endl;

    return 0;
}
