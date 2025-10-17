#include <iostream>
#include <thread>
#include <mutex> // For std::mutex and std::lock_guard
using namespace std;

mutex cookie_mutex;

void task1() { // Prints out odd numbers

    lock_guard<mutex> guard(cookie_mutex); // Lock the mutex for this scope
    for (int i = 0; i < 10; i++) {
        cout << "Tako 1 - Count: WAH " << i * 2 + 1 << endl;
    }
}

void task2() { // Prints out even numbers

    lock_guard<mutex> guard(cookie_mutex); // Lock the mutex for this scope
    for (int i = 0; i < 10; i++) {
        cout << "Tako 2 - Count: WAH " << i * 2 << endl;

    }
}

int main() {
    cout << "Initiating Takolization..." << endl;

    thread t1(task1); // Start task1 in a new thread
    thread t2(task2); // Start task2 in a new thread

    t1.join(); // Wait for task1 to finish
    t2.join(); // Wait for task2 to finish

    cout << "Both tako are complete." << endl;
    return 0;
}