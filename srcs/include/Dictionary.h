#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <locale>

inline void leftTrim(std::string &str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rightTrim(std::string &str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

inline void trim(std::string &str) {
    rightTrim(str);
    leftTrim(str);
}

struct Word {
    Word() {}
    Word(std::string s) : word(s) {}
    int getLength() const { return word.length(); }
    std::string word;
};

typedef std::vector<Word*> Words;
typedef std::unordered_map<std::string, Words> WordMap;

class Dictionary {
public:
    Dictionary();
    ~Dictionary();
    const Words* findWord(const std::string& s) const;
    bool isWord(std::string s) const;
    std::string getWord(int i) const;
    void createDefaultHash(Word* w);
    void parseFile(std::string fileName, int maxSize);
    void debugBuckets() const;
    void printWords();

private:
    Words words_;
    WordMap wordMap_;
    int maxSize_;
};