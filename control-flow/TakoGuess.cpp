#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    // Random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    int secretNumber = rand() % 100 + 1;
    int guess = 0;
    int attempts = 0;
    
    cout << "Welcome to the Tako Guessing Game!" << endl;
    cout << "I'm thinking of a number between 1 and 100 cookies." << endl;
    
    // Loops until correct
    while (guess != secretNumber) {
        cout << "Enter your guess: ";
        cin >> guess;
        attempts++;
        
        // checks if secretnumber is correct or not
        if (guess < secretNumber) {
            cout << "Too low! Try again." << endl;
        } else if (guess > secretNumber) {
            cout << "Too high! Try again." << endl;
        } else {
            cout << "Congratulations! You guessed it in " 
                      << attempts << " attempts!" << endl;
        }
    }
    
    return 0;
}