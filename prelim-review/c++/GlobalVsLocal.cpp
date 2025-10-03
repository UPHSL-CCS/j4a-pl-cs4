#include <iostream>
using namespace std;

int globalVar = 10;
 
void printGlobal() {

    cout << "Global variable value: " << globalVar << endl;
    
}
int main() {
    int localVar = 20; 
    cout << "Local variable value (inside main): " << localVar << endl;
    cout << "Global variable value (inside main): " << globalVar << endl;
    
    globalVar = 30;
    
    printGlobal();

    return 0;
}