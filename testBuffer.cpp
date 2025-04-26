#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "boundedBuffer.h"

using namespace std;

// Producer function
void producer(boundedBuffer &buffer, const vector<string> &itemsToProduce)
{
    for (const string &item : itemsToProduce)
    {
        buffer.add(item);                                  // Add string to the buffer
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate work
    }
}

// Consumer function
void consumer(boundedBuffer &buffer, int itemsToConsume)
{
    for (int i = 0; i < itemsToConsume; ++i)
    {
        string item = buffer.remove(); // Remove string from the buffer
        // cout << "Consumed: " << item << endl; // Print the consumed string
        this_thread::sleep_for(chrono::milliseconds(150)); // Simulate work
    }
}

int main()
{
    // Create a bounded buffer with a capacity of 5
    boundedBuffer buffer(5);

    // Strings to produce (e.g., file names or lines of text)
    vector<string> itemsToProduce = {
        "file1.txt",
        "file2.txt",
        "file3.txt",
        "file4.txt",
        "file5.txt",
        "file6.txt",
        "file7.txt",
        "file8.txt",
        "file9.txt",
        "file10.txt"};

    // Number of items to consume
    int itemsToConsume = itemsToProduce.size();

    // Create producer and consumer threads
    thread producerThread(producer, ref(buffer), ref(itemsToProduce));
    thread consumerThread(consumer, ref(buffer), itemsToConsume);

    // Wait for both threads to finish
    producerThread.join();
    consumerThread.join();

    return 0;
}