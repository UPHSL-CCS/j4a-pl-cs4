#include <iostream>
#include <thread>
using namespace std;

void task1() {
    for (int i = 0; i < 5; i++) {
        cout << "Tako 1 - Count: WAH " << i << endl;
    }
}

void task2() {
    for (int i = 0; i < 5; i++) {
        cout << "Tako 2 - Count: WAH " << i << endl;
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