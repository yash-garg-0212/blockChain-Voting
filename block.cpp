#include "block.h"
#include <ctime>
#include <sstream>
#include <functional>
#include <iostream>
#include <iomanip>

using namespace std;

bool debugMining = false;

Block::Block(int idx, const string &data, const string &prevHash)
    : index(idx), data(data), prevHash(prevHash), nonce(0) {
    time_t now = time(0);
    string ts = ctime(&now);
    timeStamp = ts.substr(0, ts.length() - 1); 
    currHash = mineBlock(1); 
}

string Block::calculateHash() const {
    stringstream ss;
    ss << index << data << timeStamp << prevHash << nonce;
    hash<string> hasher;
    size_t hashVal = hasher(ss.str());
    stringstream hex_ss;
    hex_ss << hex << setfill('0') << setw(16) << hashVal;
    return hex_ss.str();
}

string Block::mineBlock(int diff) {
    string prefix(diff, '0');
    string hashVal;
    int iterations = 0;
    do {
        nonce++;
        hashVal = calculateHash();
        if (debugMining && iterations++ % 10000 == 0) {
            cout << "Mining block " << index << ": nonce=" << nonce << ", hash=" << hashVal << endl;
        }
    } while (hashVal.substr(0, diff) != prefix);
    if (debugMining) {
        cout << "Mined block " << index << " with hash: " << hashVal << endl;
    }
    return hashVal;
}