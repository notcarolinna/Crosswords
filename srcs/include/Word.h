#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>
#include <unordered_map>

struct Word {
    Word() {}
    Word(std::string s) : word(s) {}
    int len() const { return word.length(); }
    std::string word;
};

typedef std::vector<Word*> Words;
typedef std::unordered_map<std::string, Words> WordMap;

#endif // WORD_H