#pragma once
#include "block.h"
#include <vector>
#include <map>
#include <string>

class Blockchain{
private:
    std::vector<Block> chain;
    int diff;

    bool hasVoted(const std::string& voterId) const;
    static bool parseVoteLine(const std::string& data, std::string& voterIdOut, std::string& voteOut);

public:
    Blockchain();

    const Block& getLatestBlock() const;
    void addBlock(const std::string &voterId, const std::string &voteData);
    bool isChainValid() const;
    void printChain() const;
    void showVoteCounts() const;
    void saveToFile(const std::string &fileName) const;
    void loadFromFile(const std::string &fileName);
};
