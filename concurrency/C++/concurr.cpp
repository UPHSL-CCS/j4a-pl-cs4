#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable> // Include condition_variable for synchronization
using namespace std;

mutex cookie_mutex;
condition_variable cv;

bool TurnUp = true;

void task1() { // Prints out odd numbers

    for (int i = 0; i <= 10; i++) {
        unique_lock<mutex> lock(cookie_mutex); // Lock the mutex for this scope

        cv.wait(lock, [] { return TurnUp; }); // Wait until it's this task's turn

        cout << "Tako 1 - Count: " << i * 2 + 1 << endl;

        TurnUp = false; // Switch turn to task2
        lock.unlock();
        cv.notify_one(); // Notify task2
    }
}

void task2() { // Prints out even numbers

    for (int i = 1; i <= 10; i++) {
        unique_lock<mutex> lock(cookie_mutex); // Lock the mutex for this scope

        cv.wait(lock, [] { return !TurnUp; }); // Wait until it's this task's turn

        cout << "Tako 2 - Count: " << i * 2 << endl;

        TurnUp = true; // Switch turn to task1
        lock.unlock();
        cv.notify_one(); // Notify task1
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