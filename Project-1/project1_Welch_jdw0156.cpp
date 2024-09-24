//=============================================================================
// Name        : project1_Welch_jdw0156.cpp
// Author      : John Welch (jdw0156)
// Version     : 09/06/2024
// Description : COMP 2710 Software Construction Project 1
// Compile     : g++ project1_Welch_jdw0156.cpp -o project1_Welch_jdw0156
//=============================================================================

#include <iostream>
using namespace std;

int main() {
    cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

    float balance = 0;
    float interest = -100;
    float monthlyPayment = 0;
    int months = 0;
    float interestPaid = 0;

    while (balance <= 0) {
		cout << "\nLoan Amount: ";
		cin >> balance;
	}

	while (interest <= -1) {
		cout << "Interest Rate (% per year): ";
		cin >> interest;
	}

    float interestR = interest / 12 / 100;


    
    cout << "Monthly Payments: ";
    cin >> monthlyPayment;
    
    cout << endl;


    cout << "*****************************************************************\n"
	     << "\tAmortization Table\n"
	     << "*****************************************************************\n"
	     << "Month\tBalance\t  Payment\tRate\tInterest\tPrincipal\n";

    while (balance > 0) {
        if (months == 0) {
            cout << months++ << "\t$" << balance;
		    if (balance < 1000) cout << "\t";
			cout << "  N/A" << "\t\t" << "N/A\tN/A\t\tN/A\n";
        }
        else {
        float interestPayment = balance * interestR;
        float actualPayment = monthlyPayment - interestPayment;
        interestPaid = interestPaid + interestPayment;
        balance = balance - (monthlyPayment - interestPayment);
        if (balance < 0) {
            actualPayment = balance + actualPayment;
			monthlyPayment = balance + monthlyPayment;
			balance = 0;
        }
		cout << months++ << "\t$" << balance;
		if (balance < 1000) cout << "\t";
			cout << "  $" << monthlyPayment << "\t" << interestR * 100 << "\t$" << interestPayment << "\t\t$" << actualPayment << "\n"; 
        }
    }


    cout << "****************************************************************\n";
	cout << "\nIt takes " << --months << " months to pay off "
	     << "the loan.\n"
	     << "Total interest paid is: $" << interestPaid;
	cout << endl << endl;

	return 0;

}
