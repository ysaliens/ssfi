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
SSFI was designed for speed and scalability. On my local virtual machine, it counted 100 MB worth of text files (in chunks of 6 MB books) in ~8 seconds multithreaded. More performance metrics to be added.

## Architecture
Due to the scope of the task, the design of SSFI has been kept simple. There are no classes, logging, or a lot of tests. This is deliberate - if the project is ever grown it would be very easy to adapt it and less code helps readibility/maintainablity.

## Testing

## Future Work and improvements
Even though each thread keeps a separate hash map for the duration of processing, the hash maps have to eventually be merged in order to compute metrics. The main thread does this at the end. An optimization would be to switch to using a concurrent hash map package that allows all threads to access the same global hash map without the need for locks. This would bring improvements to speed and memory usage as we wouldn't keep multiple hash maps or merge them. Switching away from using C++ strings (tolower) may bring additional speed.
