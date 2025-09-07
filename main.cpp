#include "blockchain.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>

using namespace std;

static void printHeader() {
    cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << "\n";
    cout << "|      Blockchain Voting System       |\n";
    cout << setfill('=') << setw(40) << "" << setfill(' ') << "\n";
}

static void printMenu() {
    cout << "\n1. Cast Vote\n"
            "2. Show Blockchain\n"
            "3. Validate Blockchain\n"
            "4. Show Vote Counts\n"
            "5. Save & Exit\n"
            "Choose: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Blockchain election;
    // Attempt to load existing data
    election.loadFromFile("blockchain.txt");

    while (true) {
        printHeader();
        printMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume newline

        if (choice == 1) {
            string voterId;
            cout << "Enter 10-digit Voter ID: ";
            getline(cin, voterId);

            cout << "Choose candidate (A/B/C): ";
            string cand; getline(cin, cand);
            if (cand == "A" || cand == "B" || cand == "C") {
                election.addBlock(voterId, cand);
            } else {
                cout << "Invalid candidate. Use A, B, or C.\n";
            }
        } else if (choice == 2) {
            election.printChain();
        } else if (choice == 3) {
            cout << (election.isChainValid() ? "\nBlockchain is VALID.\n" : "\nBlockchain is NOT valid!\n");
        } else if (choice == 4) {
            election.showVoteCounts();
        } else if (choice == 5) {
            election.saveToFile("blockchain.txt");
            cout << "\nSaved to blockchain.txt. Exiting...\n";
            break;
        } else {
            cout << "Please choose 1-5.\n";
        }
        cout << "\nPress Enter to continue..."; cin.get();
    }
    return 0;
}
