# Super Simple File Indexer (SSFI)
A fast multi-threaded word frequency counter written in C++. SSFI recursively scans a given directory for files ending in .txt. It then queues those files to be processed by worker threads. Worker threads will pick up files from the queue and scan them, counting how many occurences of each word they see. Once complete, the program prints the top 10 words and their counts.

## Features
* __Unordered__ hash maps for storing word frequency. Each thread gets it's own hash map to count words. When processing is complete, main thread merges the hash maps into a master hash map. This allows for very efficient processing and avoids the need for locks or synchronization. This design also allows the application to scale well with additional threads.
* Thread pool. Configurable number of worker threads are part of a thread pool with a queue. Worker threads use thread IDs to accumulate data into the same hash map for the duration of processing. This avoids allocating new memory for hash maps every time a worker thread processes a file.

## Building
Execute "make" from root directory. 
SSFI requires the C++ Boost 1.66 or higher library.

## Command Line Options
The following is configurable:
* -p: Path to recursively index
* -t: Number of worker threads to use
* -w: Top N words to print
* -h: Help menu

## Performance
SSFI was designed for speed and scalability. On a virtual machine with 8 cores, it counted 100 MB worth of text (in chunks of 6 MB books) in ~8 seconds multithreaded. Performance stats for 100MB of text below:  
  
Threads | Total Time (Unordered Map) | Total Time (Ordered Map)
------------ | ------------- | -------------
1 | 10.553s | 22.078s
2 | 11.316s | 17.662s
4 | 7.905s | 15.003s
8 | 8.303s | 15.927s
16 | 9.814s | 17.099s
32 | 8.173s | 15.836s
  
  
As can be seen from the tables, using unsorted maps brings in a 2x speed improvement.  
The optimal number of threads (for my VM) also seems to be 4.

## Architecture
Due to the scope of the task, the design of SSFI has been kept simple. There are no classes, logging, or a lot of tests.  
This is deliberate - if the project is ever grown it would be very easy to adapt it and less code helps readibility/maintainablity.

## Testing
When compiled, the test executable `test_worker` is located under `test/`. Execute from the `test/` directory.  
The tests cover the worker code - namely counting words in a single file.  

A full test can be done by running the ssfi executable with `./ssfi -p files`.  
If files directory has not been altered and the code works, the words will match the frequency:  
![Test](/files/test.png)

## Future Work
Even though each thread keeps a separate hash map for the duration of processing, the hash maps have to eventually be merged in order to compute metrics. The main thread does this at the end. An optimization would be to switch to using a concurrent hash map package that allows all threads to access the same global hash map without the need for locks. This would bring improvements to speed and memory usage as we wouldn't keep multiple hash maps or merge them. Switching away from using C++ strings (tolower) may bring additional speed. If the project is grown, additional tests should be added.
