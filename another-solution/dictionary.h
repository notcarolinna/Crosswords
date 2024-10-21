#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>


#include "slot.h"

class Dictionary {

public:
    Dictionary();
    void parseFile(std::string fileName, std::unordered_set<int> sizes);
    std::vector<std::string> getWordsByLength(int length) const;

private:
    std::map<int, std::vector<std::string>> _wordMap;

    void leftTrim(std::string &str);
    void rightTrim(std::string &str);
    void trim(std::string &str);

};

#endif // DICTIONARY_H