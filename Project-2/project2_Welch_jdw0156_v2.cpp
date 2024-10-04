/*
 * Author: John Welch
 * ID: jdw0156
 * Filename: project2_Welch_jdw0156_v2.cpp
 * Compile: g++ project2_Welch_jdw0156_v2.cpp -o project2_Welch_jdw0156_v2
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

// *** Unit Tests ***

// Test at_least_two_alive function
void test_at_least_two_alive(void) {
    cout << "Unit Testing 1: Function - at_least_two_alive()\n";

    // Case One
    cout << "\t Case 1: Aaron alive, Bob alive, Charlie alive\n";
    assert(true == at_least_two_alive(true, true, true));
    cout << "\tCase passed ...\n";

    // Case Two
    cout << "\t Case 2: Aaron dead, Bob alive, Charlie alive\n";
    assert(true == at_least_two_alive(false, true, true));
    cout << "\tCase passed ...\n";

    // Case Three
    cout << "\t Case 3: Aaron alive, Bob dead, Charlie alive\n";
    assert(true == at_least_two_alive(true, false, true));
    cout << "\tCase passed ...\n";

    // Case Four
    cout << "\t Case 4: Aaron alive, Bob alive, Charlie dead\n";
    assert(true == at_least_two_alive(true, true, false));
    cout << "\tCase passed ...\n";

    // Case Five
    cout << "\t Case 5: Aaron dead, Bob dead, Charlie alive\n";
    assert(false == at_least_two_alive(false, false, true));
    cout << "\tCase passed ...\n";

    // Case Six
    cout << "\t Case 6: Aaron dead, Bob alive, Charlie dead\n";
    assert(false == at_least_two_alive(false, true, false));
    cout << "\tCase passed ...\n";

    // Case Seven
    cout << "\t Case 7: Aaron alive, Bob dead, Charlie dead\n";
    assert(false == at_least_two_alive(true, false, false));
    cout << "\tCase passed ...\n";

    // Case Eight
    cout << "\t Case 8: Aaron dead, Bob dead, Charlie dead\n";
    assert(false == at_least_two_alive(false, false, false));
    cout << "\tCase passed ...\n";

    press_to_continue();
}



// Test Aaron_shoots1 function
void test_Aaron_shoots1(void) {
    cout << "Unit Testing 2: Function - Aaron_shoots_1()\n";
    
    // Case One
    bool B_alive = true; bool C_alive = true;
    cout << "\tCase 1: Bob alive, Charlie alive\n";
    Aaron_shoots1(B_alive, C_alive);
    assert(B_alive == true);
    cout << "\t\tAaron is shooting at Charlie\n";

    // Case Two
    B_alive = false; C_alive = true;
    cout << "\tCase 2: Bob dead, Charlie alive\n";
    Aaron_shoots1(B_alive, C_alive);
    assert(B_alive == false);
    cout << "\t\tAaron is shooting at Charlie\n";

    // Case Three
    B_alive = true; C_alive = false;
    cout << "\tCase 3: Bob alive, Charlie dead\n";
    Aaron_shoots1(B_alive, C_alive);
    assert(C_alive == false);
    cout << "\t\tAaron is shooting at Bob\n";
    
    press_to_continue();
}

// Test Bob_shoots function
void test_Bob_shoots(void) {
    cout << "Unit Testing 3: Function - Bob_shoots()\n";
    
    // Case One
    bool A_alive = true; bool C_alive = true;
    cout << "\tCase 1: Aaron alive, Charlie alive\n";
    Bob_shoots(A_alive, C_alive);
    assert(A_alive == true);
    cout << "\t\tBob is shooting at Charlie\n";

    // Case Two
    A_alive = false; C_alive = true;
    cout << "\tCase 2: Aaron dead, Charlie alive\n";
    Bob_shoots(A_alive, C_alive);
    assert(A_alive == false);
    cout << "\t\tBob is shooting at Charlie\n";

    // Case Three
    A_alive = true; C_alive = false;
    cout << "\tCase 3: Aaron alive, Charlie dead\n";
    Bob_shoots(A_alive, C_alive);
    assert(C_alive == false);
    cout << "\t\tBob is shooting at Aaron\n";
    
    press_to_continue();
}

// Test Charlie_shoots function
void test_Charlie_shoots(void) {
    cout << "Unit Testing 4: Function - Charlie_shoots()\n";
    
    bool A_alive = true; bool B_alive = true;
    cout << "\tCase 1: Aaron alive, Bob alive\n";
    Charlie_shoots(A_alive, B_alive);
    assert(A_alive == true);
    cout << "\t\tCharlie is shooting at Bob\n";

    A_alive = false; B_alive = true;
    cout << "\tCase 2: Aaron dead, Bob alive\n";
    Charlie_shoots(A_alive, B_alive);
    assert(A_alive == false);
    cout << "\t\tCharlie is shooting at Bob\n";

    A_alive = true; B_alive = false;
    cout << "\tCase 3: Aaron alive, Bob dead\n";
    Charlie_shoots(A_alive, B_alive);
    assert(B_alive == false);
    cout << "\t\tCharlie is shooting at Aaron\n";
    
    press_to_continue();
}

// Test Aaron_shoots2 function
void test_Aaron_shoots2(void) {
    cout << "Unit Testing 5: Function - Aaron_shoots_2()\n";

    // Case One
    bool bob_alive = true; bool charlie_alive = true;
    cout << "\tCase 1: Bob alive, Charlie alive\n";
    Aaron_shoots2(bob_alive, charlie_alive);
    assert(bob_alive == true);
    cout << "\t\tAaron intentionally misses his first shot\n";
    cout << "\t\tBoth Bob and Charlie are alive.\n";

    // Case Two
    bob_alive = false; charlie_alive = true;
    cout << "\tCase 2: Bob dead, Charlie alive\n";
    Aaron_shoots2(bob_alive, charlie_alive);
    assert(bob_alive == false);
    cout << "\t\tAaron is shooting at Charlie\n";

    // Case Three
    bob_alive = true; charlie_alive = false;
    cout << "\tCase 3: Bob alive, Charlie dead\n";
    Aaron_shoots2(bob_alive, charlie_alive);
    assert(charlie_alive == false);
    cout << "\t\tAaron is shooting at Bob\n";

    press_to_continue();
}

int main() {
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);
    srand(static_cast<unsigned int>(time(0)));
    rand(); 
    cout << "*** Welcome to John's Duel Simulator ***\n";
    test_at_least_two_alive();
    test_Aaron_shoots1();
    test_Bob_shoots();
    test_Charlie_shoots();
    test_Aaron_shoots2();
    truel();  
    return 0;
}
