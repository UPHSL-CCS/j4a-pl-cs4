#include <iostream>
using namespace std;

int globalVar = 10;
 
void printGlobal() {

    cout << "Global variable value: " << globalVar << std::endl;
    
}
int main() {
    int localVar = 20; 
    cout << "Local variable value (inside main): " << localVar << std::endl;
    cout << "Global variable value (inside main): " << globalVar << std::endl;
    
    globalVar = 30;
    
    printGlobal();

    return 0;
}