#include <iostream>
using namespace std;

int main() {
    int x = 5;
    int y = 10;
    
    if (x = y) {
        cout << "x is greater than y" << std::endl; 
    } else {
        cout << "x is not greater than y" << std::endl;
    }
    
    cout << "x is now: " << x;
    
    return 0;
}