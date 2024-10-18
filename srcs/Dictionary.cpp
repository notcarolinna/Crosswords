#include "../include/Dictionary.h"

Dictionary::Dictionary() {}

Dictionary::~Dictionary() {
    for (Word* w : words_) {
        delete w;
    }
}

const Words* Dictionary::findWord(const std::string& s) const {
    auto it = wordMap_.find(s);
    if (it != wordMap_.end()) {
        return &it->second;
    }
    return NULL;
}

bool Dictionary::isWord(std::string s) const {
    auto it = wordMap_.find(s);
    if (it == wordMap_.end()) {
        return false;
    } else {
        return true;
    }
}

std::string Dictionary::getWord(int i) const {
    assert(i >= 0 && i < words_.size());
    return words_[i]->word;
}

void Dictionary::createDefaultHash(Word* w) {
    int length = w->getLength();
    if (length > maxSize_) return;
    int numPatterns = 1 << length;
    for (int i = 0; i < numPatterns; i++) {
        std::string temp = w->word;
        for (int j = 0; j < length; j++) {
            if ((i >> j) & 1) {
                temp[j] = '?';
            }
        }
        wordMap_[temp].push_back(w);
    }
}

void Dictionary::parseFile(std::string fileName, int maxSize) {
    maxSize_ = maxSize;
    std::ifstream file;
    file.open(fileName);
    while (file.is_open() && !file.eof()) {
        std::string line;
        std::getline(file, line);
        if (!line.empty()) {
            trim(line);
            int length = line.length();
            if (line[length - 1] == '\r') {
                line = line.substr(0, length - 1);
            }
            length = line.length();
            if (std::all_of(line.begin(), line.end(), [](char c) { return std::isalpha(c); })) {
                if (length <= maxSize_) {
                    Word* w = new Word(line);
                    words_.push_back(w);
                    createDefaultHash(w);
                }
            }
        }
    }
    std::cout << "Read " << words_.size() << " words" << std::endl;
}

void Dictionary::debugBuckets() const {
    for (int i = 0; i < wordMap_.bucket_count(); i++) {
        std::cout << "[" << i << "] " << wordMap_.bucket_size(i) << std::endl;
    }
}

void Dictionary::printWords() {
    for (Word* w : words_) {
        std::cout << w->word << std::endl;
    }
}