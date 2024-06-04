/*
Author: Dexter Jones

- Program that exhibits concurrency concepts
- Creates two threads that act as counters
- One thread counts up to 20
- Once Thread one reaches 20, second thread counts down to 0
- Provide detailed analysis of appropriate concepts that impact application, address:
	- Performance issues with concurrency
	- Vulnerabilities exhibited with use of strings
	- Security of the data types exhibited
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv; // used to synchronize threads
bool thread1Done = false;

void countUp() {
    cout << "Count up: " << endl;

    for (int i = 0; i <= 20; ++i) {
        this_thread::sleep_for(chrono::milliseconds(100)); // pause for effect
        lock_guard<mutex> lock(mtx); // lock mutex
        cout << i << endl;
    }
    {
        lock_guard<mutex> lock(mtx); // lock for thread1Done
        thread1Done = true;
    }
    cv.notify_one(); // Notifies second thread to start
}

void countDown() {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [] { return thread1Done; });

    cout << "Count down:" << endl;
    for (int i = 20; i >= 0; --i) {
        this_thread::sleep_for(chrono::milliseconds(100)); // pause for effect
        cout << i << endl;
    }
}

int main() {
    thread t1(countUp);
    thread t2(countDown);

    // join threads
    t1.join();
    t2.join();
    cout << "Blast off!" << endl;

    return 0;
}
