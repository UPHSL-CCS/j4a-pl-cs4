#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

mutex cookie_mutex;

void task1() {

    lock_guard<mutex> guard(cookie_mutex);
    for (int i = 0; i < 5; i++) {
        cout << "Tako 1 - Count: WAH " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void task2() {

    lock_guard<mutex> guard(cookie_mutex);
    for (int i = 0; i < 5; i++) {
        cout << "Tako 2 - Count: WAH " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    cout << "Initiating Takolization..." << endl;

    thread t1(task1);
    thread t2(task2);

    t1.join();
    t2.join();

    cout << "Both tako are complete." << endl;
    return 0;
}