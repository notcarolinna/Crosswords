#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Word.h"

class Library {
public:
    Library() {}
    ~Library();

    const Words* FindWord(const std::string& s) const;
    bool IsWord(std::string s) const;
    void ComputeStats();
    void PrintStats();
    std::string GetWord(int i) const;
    void CreatePatternHash(Word* w);
    void ReadFromFile(std::string fileName, int max_size);
    void DebugBuckets() const;

private:
    Words words_;
    WordMap word_map_;
    std::vector<int> counts_;
};

extern Library lib;

#endif