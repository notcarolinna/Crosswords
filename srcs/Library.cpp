#include "Library.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

Library lib;

Library::~Library() {
    for (Word* w : words_) {
        delete w;
    }
}

const Words* Library::FindWord(const std::string& s) const {
    auto it = word_map_.find(s);
    if (it != word_map_.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}

bool Library::IsWord(std::string s) const {
    auto it = word_map_.find(s);
    return it != word_map_.end();
}

void Library::ComputeStats() {
    assert(counts_.empty());
    int maxSize = 25;
    counts_.resize(maxSize);
    for (const Word* w : words_) {
        int len = w->word.length();
        if (len < maxSize) {
            counts_[len]++;
        }
    }
}

void Library::PrintStats() {
    std::cout << "counts of each word length" << std::endl;
    for (int i = 1; i < counts_.size(); i++) {
        std::cout << i << ") " << counts_[i] << std::endl;
    }
}

std::string Library::GetWord(int i) const {
    assert(i >= 0 && i < words_.size());
    return words_[i]->word;
}

void Library::CreatePatternHash(Word* w) {
    int len = w->len();
    if (len > 7) {
        return;
    }
    int num_patterns = 1 << len;
    for (int i = 0; i < num_patterns; i++) {
        std::string temp = w->word;
        for (int j = 0; j < len; j++) {
            if ((i >> j) & 1) {
                temp[j] = '?';
            }
        }
        word_map_[temp].push_back(w);
    }
}

void Library::ReadFromFile(std::string fileName, int max_size) {
    std::ifstream file;
    file.open(fileName);
    while (file.is_open() && !file.eof()) {
        std::string line;
        std::getline(file, line);
        if (!line.empty()) {
            int len = line.length();
            if (std::all_of(line.begin(), line.end(), [](char c) { return std::isalpha(c); })) {
                if (line.length() <= max_size) {
                    Word* w = new Word(line);
                    words_.push_back(w);
                    CreatePatternHash(w);
                }
            }
        }
    }
    std::cout << "Read " << words_.size() << " words" << std::endl;
}

void Library::DebugBuckets() const {
    for (int i = 0; i < word_map_.bucket_count(); i++) {
        std::cout << "[" << i << "] " << word_map_.bucket_size(i) << std::endl;
    }
}