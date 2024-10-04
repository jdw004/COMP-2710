/*
 * Author: John Welch
 * ID: jdw0156
 * Filename: project2_Welch_jdw0156.cpp
 * Compile: g++ project2_Welch_jdw0156.cpp -o project2_Welch_jdw0156
 * Resources: Internet for C++ syntax and Dr Li slides
 */
 
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <ctime>
#include <cstdlib>
using namespace std;

const double aOdds = 0.33333333;
const double bOdds = 0.5;
const double cOdds = 1.0;

const int numDuels = 10000;

bool fire(double odds) {
    // Generate a random number between 0 and 1 and compare it to player odds
    double randy = static_cast<double>(rand()) / RAND_MAX;
    return randy <= odds;
}

bool at_least_two_alive(bool A_alive, bool B_alive, bool C_alive) {
    // Checks if any two players are alive
    return (A_alive && (B_alive || C_alive)) || (B_alive && C_alive);
}

void Aaron_shoots1(bool& B_alive, bool& C_alive) {
    // Aaron shoots at the player with the highest priority
    if (C_alive) {
        bool isHit = fire(aOdds);
        if (isHit) {
            C_alive = false; 
        }
    } 
    // Aaron shoots at remaining player
    else if (B_alive) {
        bool isHit = fire(aOdds);
        if (isHit) {
            B_alive = false; 
        }
    }
}

void Bob_shoots(bool& A_alive, bool& C_alive) {
    // Bob shoots at the player with the highest priority
    if (C_alive) {
        bool isHit = fire(bOdds);
        if (isHit) {
            C_alive = false; 
        }
    } 
    // Bob shoots at remaining player
    else if (A_alive) {
        bool isHit = fire(bOdds);
        if (isHit) {
            A_alive = false; 
        }
    }
}

void Charlie_shoots(bool& A_alive, bool& B_alive) {
    // Charlie shoots at the player with the highest priority
    if (B_alive) {
        bool isHit = fire(cOdds);
        if (isHit) {
            B_alive = false;
        }
    } 
    // Charlie shoots at remaining player
    else if (A_alive) {
        bool isHit = fire(cOdds);
        if (isHit) {
            A_alive = false; 
        }
    }
}

void Aaron_shoots2(bool& B_alive, bool& C_alive) {
    // Aaron purposely misses if both players are alive
    if (B_alive && C_alive) {
        return; 
    } 
    // Aaron shoots at remaining player
    else {
        Aaron_shoots1(B_alive, C_alive);
    }
}

void press_to_continue() {
    cout << "Press any key to continue...";
    cin.ignore().get();
}

// Discover which strategy is better
void weigh_strategies(int aWinsOne, int aWinsTwo) {
    if (aWinsOne > aWinsTwo) {
        cout << "Strategy 1 is better than strategy 2\n";
    } else if (aWinsTwo > aWinsOne) {
        cout << "Strategy 2 is better than strategy 1\n";
    } else {
        cout << "Both strategies produce equal results for Aaron\n";
    }
}

void truel() {
    bool A_alive = true;
    bool B_alive = true;
    bool C_alive = true;
    int aWinsOne = 0;
    int aWinsTwo = 0;  
    int bobWins = 0;
    int charlieWins = 0;

    cout << "Ready to test strategy 1 (run 10000 times):" << endl;
    press_to_continue();

    // Test Strategy One 10,000 times
    for (int i = 0; i < 10000; i++) {
        A_alive = true;
        B_alive = true;
        C_alive = true;
        while (at_least_two_alive(A_alive, B_alive, C_alive)) {
            if (A_alive) {
                Aaron_shoots1(B_alive, C_alive);
            }
            if (B_alive) {
                Bob_shoots(A_alive, C_alive);
            }
            if (C_alive) {
                Charlie_shoots(A_alive, B_alive);
            }
        }
        if (A_alive) {
            aWinsOne++;
        } else if (B_alive) {
            bobWins++;
        } else if (C_alive) {
            charlieWins++;
        }
    }

    // Strategy One Results
    cout << "Aaron won " << aWinsOne << "/10000 duels or " 
         << (static_cast<double>(aWinsOne) / 10000) * 100 << "%\n";
    cout << "Bob won " << bobWins << "/10000 duels or " 
         << (static_cast<double>(bobWins) / 10000) * 100 << "%\n";
    cout << "Charlie won " << charlieWins << "/10000 duels or " 
         << (static_cast<double>(charlieWins) / 10000) * 100 << "%\n";
    cout << endl;
    
    // Test Strategy Two 10,000 times
    cout << "Ready to test strategy 2 (run 10000 times):" << endl;
    press_to_continue();

    aWinsTwo = 0;
    bobWins = 0;
    charlieWins = 0;

    for (int i = 0; i < 10000; i++) {
        A_alive = true;
        B_alive = true;
        C_alive = true;
        while (at_least_two_alive(A_alive, B_alive, C_alive)) {
            if (A_alive) {
                Aaron_shoots2(B_alive, C_alive);
            }
            if (B_alive) {
                Bob_shoots(A_alive, C_alive);
            }
            if (C_alive) {
                Charlie_shoots(A_alive, B_alive);
            }
        }
        if (A_alive) {
            aWinsTwo++;
        } else if (B_alive) {
            bobWins++;
        } else if (C_alive) {
            charlieWins++;
        }
    }

    // Strategy Two Results
    cout << "Aaron won " << aWinsTwo << "/10000 duels or " 
         << (static_cast<double>(aWinsTwo) / 10000) * 100 << "%\n";
    cout << "Bob won " << bobWins << "/10000 duels or " 
         << (static_cast<double>(bobWins) / 10000) * 100 << "%\n";
    cout << "Charlie won " << charlieWins << "/10000 duels or " 
         << (static_cast<double>(charlieWins) / 10000) * 100 << "%\n";
    cout << endl;

    // Comprehensive Results
    weigh_strategies(aWinsOne, aWinsTwo);
}

int main() {
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);
    srand(static_cast<unsigned int>(time(0)));
    rand(); 
    cout << "*** Welcome to John's Duel Simulator ***\n";
    truel();  
    return 0;
}
