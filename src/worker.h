// Count word frequencies in a file
void count(std::string path, std::unordered_map<std::string, int> &wordCounts);

// Get thread ID and process a file
void process(std::string path, std::atomic<int>& id, std::vector<std::unordered_map<std::string, int>> &maps);
