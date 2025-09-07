#pragma once
#include<string>
using namespace std;

class Block{
    public:
    int index;
    string data;
    string timeStamp;
    string prevHash;
    string currHash;
    int nonce;

    Block(int idx, const string& data, const string& prevHash);
    string calculateHash() const;
    string mineBlock(int diff);
};