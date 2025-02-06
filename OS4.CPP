#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

class Buffer {
private:
    queue<int> buffer;  // Shared buffer
    int maxSize;        // Maximum buffer size
    mutex mtx;          // Mutex for mutual exclusion
    condition_variable cvFull;   // Condition variable to wait for buffer to be full
    condition_variable cvEmpty;  // Condition variable to wait for buffer to be empty

public:
    Buffer(int size) : maxSize(size) {}

    // Producer function to add data to the buffer
    void produce(int item) {
        unique_lock<mutex> lock(mtx);  // Acquire lock to ensure mutual exclusion
        // Wait if the buffer is full
        cvFull.wait(lock, [this]() { return buffer.size() < maxSize; });
        buffer.push(item);
        cout << "Produced: " << item << endl;
        // Notify the consumer that there is new data
        cvEmpty.notify_one();
    }

    // Consumer function to remove data from the buffer
    int consume() {
        unique_lock<mutex> lock(mtx);  // Acquire lock to ensure mutual exclusion
        // Wait if the buffer is empty
        cvEmpty.wait(lock, [this]() { return !buffer.empty(); });
        int item = buffer.front();
        buffer.pop();
        cout << "Consumed: " << item << endl;
        // Notify the producer that there is space in the buffer
        cvFull.notify_one();
        return item;
    }
};

// Producer function to simulate producing items
void producer(Buffer& buffer, int id) {
    int item = 0;
    while (true) {
        item++;
        buffer.produce(item);  // Produce item and put it in the buffer
        this_thread::sleep_for(chrono::milliseconds(100));  // Simulate work
    }
}

// Consumer function to simulate consuming items
void consumer(Buffer& buffer, int id) {
    while (true) {
        buffer.consume();  // Consume item from the buffer
        this_thread::sleep_for(chrono::milliseconds(150));  // Simulate work
    }
}

int main() {
    int bufferSize = 5;  // Buffer size
    Buffer buffer(bufferSize);

    // Create 3 producers and 3 consumers
    thread producers[3];
    thread consumers[3];
    for (int i = 0; i < 3; i++) {
        producers[i] = thread(producer, ref(buffer), i);
        consumers[i] = thread(consumer, ref(buffer), i);
    }

    // Join all threads
    for (int i = 0; i < 3; i++) {
        producers[i].join();
        consumers[i].join();
    }

    return 0;
}
