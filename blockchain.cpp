#include "blockchain.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <cstring>

using namespace std;

static bool isValidVoterID(const std::string& voterId) {
    std::regex pattern("^[0-9]{10}$");
    return std::regex_match(voterId, pattern);
}

static std::string makeData(const std::string& voterId, const std::string& candidate) {
    return voterId + " -> Voted for " + candidate;
}

bool Blockchain::parseVoteLine(const std::string& data, std::string& voterIdOut, std::string& voteOut) {
    auto pos = data.find(" -> Voted for ");
    if (pos == std::string::npos) return false;
    voterIdOut = data.substr(0, pos);
    voteOut = data.substr(pos + strlen(" -> Voted for "));
    auto ltrim = [](std::string& s){ s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch){return !std::isspace(ch);})); };
    auto rtrim = [](std::string& s){ s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch){return !std::isspace(ch);}).base(), s.end()); };
    ltrim(voterIdOut); rtrim(voterIdOut);
    ltrim(voteOut); rtrim(voteOut);
    return !voterIdOut.empty() && !voteOut.empty();
}

Blockchain::Blockchain() : diff(4) {
    Block genesis(0, "Genesis Block", "0");
    genesis.currHash = genesis.mineBlock(diff);
    chain.push_back(genesis);
}

const Block& Blockchain::getLatestBlock() const {
    return chain.back();
}

bool Blockchain::hasVoted(const std::string& voterId) const {
    for (size_t i = 1; i < chain.size(); ++i) { // skip genesis
        std::string vid, cand;
        if (parseVoteLine(chain[i].data, vid, cand) && vid == voterId) {
            return true;
        }
    }
    return false;
}

void Blockchain::addBlock(const std::string &voterId, const std::string &voteData) {
    if (!isValidVoterID(voterId)) {
        cout << "\nError: Invalid Voter ID. It must be a 10-digit number.\n";
        return;
    }
    if (hasVoted(voterId)) {
        cout << "\nWarning: Duplicate vote prevented for voter ID " << voterId << ".\n";
        return;
    }
    const std::string data = makeData(voterId, voteData);
    Block newBlock(static_cast<int>(chain.size()), data, getLatestBlock().currHash);
    newBlock.currHash = newBlock.mineBlock(diff);
    chain.push_back(newBlock);
    cout << "\nSuccess: Vote recorded for " << voteData << ". Block index " << newBlock.index << ".\n";
}

bool Blockchain::isChainValid() const {
    std::string target(diff, '0');
    for (size_t i = 1; i < chain.size(); ++i) {
        const Block& curr = chain[i];
        const Block& prev = chain[i-1];
        if (curr.prevHash != prev.currHash) {
            cerr << "Invalid link at index " << i << ".\n";
            return false;
        }
        if (curr.calculateHash() != curr.currHash) {
            cerr << "Invalid hash at index " << i << ".\n";
            return false;
        }
        if (curr.currHash.substr(0, diff) != target) {
            cerr << "Invalid proof-of-work at index " << i << ".\n";
            return false;
        }
    }
    return true;
}

void Blockchain::printChain() const {
    cout << "\n--- Blockchain ---\n";
    for (const auto& b : chain) {
        cout << "Index: " << b.index << "\n"
             << "Time : " << b.timeStamp << "\n"
             << "Data : " << b.data << "\n"
             << "Prev : " << b.prevHash << "\n"
             << "Hash : " << b.currHash << "\n"
             << "Nonce: " << b.nonce << "\n"
             << "------------------\n";
    }
}

void Blockchain::showVoteCounts() const {
    std::map<std::string, int> counts;
    for (size_t i = 1; i < chain.size(); ++i) {
        std::string voter, cand;
        if (parseVoteLine(chain[i].data, voter, cand)) {
            counts[cand]++;
        }
    }
    if (counts.empty()) {
        cout << "\nNo votes recorded yet.\n";
        return;
    }
    size_t nameW = 0; int total = 0;
    for (const auto& kv : counts) { nameW = std::max(nameW, kv.first.size()); total += kv.second; }
    cout << "\nVote Counts (Total " << total << ")\n";
    cout << left << setw((int)nameW+2) << "Candidate" << right << setw(8) << "Votes" << "\n";
    cout << string((int)nameW+2+8, '-') << "\n";
    for (const auto& kv : counts) {
        cout << left << setw((int)nameW+2) << kv.first << right << setw(8) << kv.second << "\n";
    }
}

void Blockchain::saveToFile(const std::string &filename) const {
    ofstream out(filename, ios::trunc);
    if (!out) {
        cerr << "Error: Could not open " << filename << " for writing.\n";
        return;
    }
    for (const auto& b : chain) {
        out << b.index << "\n";
        out << b.data << "\n";
        out << b.timeStamp << "\n";
        out << b.prevHash << "\n";
        out << b.currHash << "\n";
        out << b.nonce << "\n";
        out << "---\n";
    }
    out.close();
}

void Blockchain::loadFromFile(const std::string &filename) {
    ifstream in(filename);
    if (!in) {
        return;
    }
    vector<Block> loaded;
    string line;
    try {
        while (true) {
            string idxLine;
            if (!std::getline(in, idxLine)) break;
            if (idxLine.empty()) continue;
            int idx = stoi(idxLine);

            string data; if (!getline(in, data)) break;
            string time; if (!getline(in, time)) break;
            string prevHash; if (!getline(in, prevHash)) break;
            string currHash; if (!getline(in, currHash)) break;
            string nonceLine; if (!getline(in, nonceLine)) break;
            int nonce = stoi(nonceLine);
            string sep; getline(in, sep);

            Block b(idx, data, prevHash);
            b.timeStamp = time;
            b.currHash = currHash;
            b.nonce = nonce;
            loaded.push_back(b);
        }
        if (!loaded.empty()) {
            chain = std::move(loaded);
        }
    } catch (const exception& e) {
        cerr << "Error: Failed to load blockchain from " << filename << ": " << e.what() << ".\n";
    }
    in.close();
}
