#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <algorithm>
using namespace std;

// -------------------------------------------------------------------------
//-Word
struct Word {
    Word() {}
    Word(string s) : word(s) {}
    int len() const { return word.length(); }
    string word;
};
typedef vector<Word*> Words;
typedef unordered_map<string, Words> WordMap;

// -------------------------------------------------------------------------
//-Library
class Library {
    public:
        Library() {}
        ~Library() {
            for (Word* w : words_) {
                delete w;
            }
        }
        void FindWord(const string& s) const {
            auto it = word_map_.find(s);
            if (it != word_map_.end()) {
                for (const Word* w : it->second) {
                    cout << " " << w->word;
                }
                cout << endl;
            }
        }
        bool IsWord(string s) const {
            auto it = word_map_.find(s);
            if (it == word_map_.end()) {
                return false;
            }
            else {
                return true;
            }
        }
        void ComputeStats() {
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
        void PrintStats() {
            cout << "counts of each word length" <<endl;
            for (int i = 1; i < counts_.size(); i++) {
                cout << i << ") " << counts_[i] << endl;
            }
        }
        string GetWord(int i) const {
            assert(i >= 0 && i < words_.size());
            return words_[i]->word;
        }
        void CreatePatternHash(Word* w) {
            int len = w->len();
            int num_patterns = 1 << len;
            //cout << "PATTERN HASH on " << w->word << endl;
            for (int i = 0; i < num_patterns; i++) {
                //cout << "  " << i << endl;
                string temp = w->word;
                for (int j = 0; j<len; j++) {
                    if ((i >> j) & 1) {
                        temp[j] = '?';
                    }
                }
                //cout << "  " << temp << endl;
                word_map_[temp].push_back(w);
            }
        }
        void ReadFromFile(string fileName) {
            ifstream file;
            file.open(fileName);
            while (file.is_open() && !file.eof()) {
                string line;
                getline(file, line);
                if (!line.empty()) {
                    if(all_of(line.begin(), line.end(), [] (char c) {return isalpha(c);} )) {
                        Word* w = new Word(line);
                        words_.push_back(w);
                        CreatePatternHash(w);
                        //word_map_[line] = Word(line);
                    }
                }
            }
            cout << "Read " << words_.size() << " words" << endl;
        }
        void DebugBuckets() const {
            for (int i = 0; i < word_map_.bucket_count(); i++) {
                cout << "[" << i << "] " << word_map_.bucket_size(i) << endl;
            }
        }
    private:
        Words words_;
        WordMap word_map_;
        vector<int> counts_;
};

// -------------------------------------------------------------------------
//-Grid
struct Grid {
    Grid() {
        name = "UNTITLED";
    }
    Grid(string n) {
        name = n;
    }
    int rows() const { return lines.size(); }
    int cols() const {
        if (lines.empty()) {
            return 0;
        }
        else {
            return lines[0].size();
        }
    }
    int LoadFromFile(string fileName) {
        ifstream file;
        file.open(fileName);
        while (file.is_open() && !file.eof()) {
            string line;
            getline(file, line);
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
    }
    int Check() const {
        int rows = lines.size();
        int cols = lines[0].size(); // could be length()
        for (int i = 3; i < lines.size(); i++) {
            assert(lines[i].size() == cols);
        }
    }
    int Print() const {
        cout << "Grid: " << name
             << " (rows=" << rows()
             << ", cols=" << cols() << ")\n";
        for (string s : lines) {
            cout << "   " << s << endl;
        }
    }
    string name;
    vector<string> lines;

};

int main() {
    Library lib;
    lib.ReadFromFile("./resources/teste2.txt");

    lib.FindWord("B???");


    //Grid grid("MY GRID");
    //grid.LoadFromFile("./resources/grid-teste.txt");
    //grid.Check();
    //grid.Print();

    return 0;
}
