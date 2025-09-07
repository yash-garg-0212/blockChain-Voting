#include <iostream>
#include "blockchain.h"
using namespace std;

int main() {
    Blockchain bChain;
    bChain.loadFromFile("blockchain.txt");

    int choice;
    while (true) {
        cout << "\n==== Blockchain Voting System ====\n";
        cout << "1. Cast Vote\n";
        cout << "2. Show Blockchain\n";
        cout << "3. Validate Blockchain\n";
        cout << "4. Show Vote Counts\n";
        cout << "5. Save & Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string voterId, candidate;
            cout << "Enter 10-digit Voter ID: ";
            cin >> voterId;
            cout << "Vote for candidate (A/B/C): ";
            cin >> candidate;
            bChain.addBlock(voterId, candidate);
        } 
        else if (choice == 2) {
            bChain.printChain();
        } 
        else if (choice == 3) {
            cout<<endl;
            cout << (bChain.isChainValid() ? "Blockchain is valid\n" : "Blockchain is NOT valid\n");
        } 
        else if (choice == 4) {
            bChain.showVoteCounts();
        } 
        else if (choice == 5) {
            bChain.saveToFile("blockchain.txt");
            cout << "Exiting... Blockchain saved.\n";
            break;
        } 
        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
