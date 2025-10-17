#include <iostream>
#include <thread>
#include <mutex> // For std::mutex and std::lock_guard
using namespace std;

mutex cookie_mutex;

void task1() {

    lock_guard<mutex> guard(cookie_mutex);
    for (int i = 0; i < 10; i++) {
        cout << "Tako 1 - Count: WAH " << i * 2 + 1 << endl;
    }
}

void task2() {

    lock_guard<mutex> guard(cookie_mutex);
    for (int i = 0; i < 10; i++) {
        cout << "Tako 2 - Count: WAH " << i * 2 << endl;

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