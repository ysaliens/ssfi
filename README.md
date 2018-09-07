# Super Simple File Indexer (SSFI)
A very fast multi-threaded word frequency counter written in C++. SSFI recursively scans a given directory for files ending in .txt. It then queues those files to be processed by worker threads. Worker threads will pick up files from the queue and scan them, counting how many occurences of each word they see. Once complete, the program prints the top 10 words and their counts.

## Features
* __Unordered__ hash maps for storing word frequency. Each thread gets it's own hash map to count words. When processing is done, main thread merges the hash maps into a master hash map. This allows for very efficient and fast processing and avoids the need for any locks or synchronization. 
* Thread pool. Configurable number of worker threads are all part of a thread pool with a queue. Each thread also keeps the same hash map while processing different text files as the threads utilize thread IDs to access the same hash maps. This avoids allocating new hash maps every time a worker thread processes a new file and aids in efficiency.
