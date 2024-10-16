#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "word.h"
#include <vector>
#include <string>

class Dictionary {
    public:
        Dictionary() {}
        ~Dictionary();
        const Words* findWord(const std::string& s) const;
        bool isWord(std::string s) const;
        void calculateStatistics();
        void printStatistics();
        std::string getWord(int i) const;
        void createDefaultHash(Word* w);
        void parseFile(std::string fileName, int maxSize);
        void debugBuckets() const;
        void printWords();

    private:
        Words words_;
        WordMap wordMap_;
        std::vector<int> counts_;
};

extern Dictionary dictionary;

#endif // DICTIONARY_H