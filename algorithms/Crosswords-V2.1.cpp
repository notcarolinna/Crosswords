#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <chrono>

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

struct Point {
    Point() {}
    Point(int r, int c) : row(r), col(c) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    int row = 0;
    int col = 0;
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << '(' << p.row << ',' << p.col << ')';
    return os;
}

struct Interval {
    Interval(Point p, int l, bool v) : point(p), length(l), vertical(v) {}
    Point getPoint(int i) const {
        assert(i >= 0 && i < length);
        if(vertical) {
            return Point(point.row+i, point.col);
        }
        else {
            return Point(point.row, point.col+i);
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Interval& s);

    Point point;
    int length;
    bool vertical;
};
typedef std::vector<Interval> Intervals;

std::ostream& operator<<(std::ostream& os, const Interval& s) {
    os << '[' << s.point << ']' << " length=" << s.length << " vertical=" << s.vertical << ')';
    return os;
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
        Dictionary() {}
        ~Dictionary() {
            for (Word* w : words_) {
                delete w;
            }
        }
        const Words* findWord(const std::string& s) const {
            auto it = wordMap_.find(s);
            if (it != wordMap_.end()) {
                return &it->second;
            } else {
                return NULL;
            }
        }
        bool isWord(std::string s) const {
            auto it = wordMap_.find(s);
            if (it == wordMap_.end()) {
                return false;
            }
            else {
                return true;
            }
        }
        void calculateStatistics() {
            assert(counts_.empty());
            int maxSize = 25;
            counts_.resize(maxSize);
            for (const Word* w : words_) {
                int length = w->word.length();
                if (length < maxSize) {
                    counts_[length]++;
                }
            }
        }
        void printStatistics() {
            std::cout << "Counts of each word length" << std::endl;
            for (int i = 1; i < counts_.size(); i++) {
                std::cout << i << ") " << counts_[i] << std::endl;
            }
        }
        std::string getWord(int i) const {
            assert(i >= 0 && i < words_.size());
            return words_[i]->word;
        }
        void createDefaultHash(Word* w) {
            int length = w->getLength();
            if (length > 7) return;
            int numPatterns = 1 << length;
            for (int i = 0; i < numPatterns; i++) {
                std::string temp = w->word;
                for (int j = 0; j<length; j++) {
                    if ((i >> j) & 1) {
                        temp[j] = '?';
                    }
                }
                wordMap_[temp].push_back(w);
            }
        }
        void parseFile(std::string fileName, int maxSize) {
            std::ifstream file;
            file.open(fileName);
            while (file.is_open() && !file.eof()) {
                std::string line;
                std::getline(file, line);
                if (!line.empty()) {
                    trim(line);
                    int length = line.length();
                    if (line[length-1] == '\r') {
                        line = line.substr(0, length-1);
                    }
                    length = line.length();
                    if(std::all_of(line.begin(), line.end(), [] (char c) {return std::isalpha(c);} )) {
                        if (length <= maxSize) {
                            Word* w = new Word(line);
                            words_.push_back(w);
                            createDefaultHash(w);
                        }
                    }
                }
            }
            std::cout << "Read " << words_.size() << " words" << std::endl;
        }
        void debugBuckets() const {
            for (int i = 0; i < wordMap_.bucket_count(); i++) {
                std::cout << "[" << i << "] " << wordMap_.bucket_size(i) << std::endl;
            }
        }
        void printWords() {
            for (Word* w : words_) {
                std::cout << w->word << std::endl;
            }
        }
    private:
        Words words_;
        WordMap wordMap_;
        std::vector<int> counts_;
};

Dictionary dictionary;

struct Attribute {
    bool isEmpty() const { return hasSpaces && !hasLetters; }
    bool isPartial() const { return hasSpaces && hasLetters; }
    bool isFull() const { return !hasSpaces && hasLetters; }
    bool hasLetters = false;
    bool hasSpaces = false;
};

struct Grid {
    Grid() {
        name = "UNTITLED";
    }
    Grid(std::string n) {
        name = n;
    }
    int getRows() const { return rows_.size(); }
    int getCols() const {
        if (rows_.empty()) {
            return 0;
        }
        else {
            return rows_[0].size();
        }
    }
    int getMaxSize() const { return std::max(getRows(), getCols()); }
    char getCell(const Point& p) const {
        assert(isInside(p));
        return rows_[p.row][p.col];
    }
    void setCell(const Point& p, char c) {
        assert(isInside(p));
        rows_[p.row][p.col] = c;
    }
    bool isBlock(const Point& p) const {
        return getCell(p) == '.';
    }
    bool isSpace(const Point& p) const {
        return getCell(p) == '?';
    }
    bool isLetter(const Point& p) const {
        char c = getCell(p);
        return c >= 'A' && c <= 'Z';
    }
    bool isInside(const Point& p) const {
        return (p.row >= 0 && p.row < getRows() && p.col >= 0 && p.col < getCols());
    }
    std::string getString(const Interval& s, Attribute& attribute) const {
        int length = s.length;
        std::string temp;
        temp.resize(length);
        for (int i = 0; i < length; i++) {
            Point p = s.getPoint(i);
            char c = getCell(p);
            if (c == '?') {
                attribute.hasSpaces = true;
            }
            else if (c >= 'A' && c <= 'Z') {
                attribute.hasLetters = true;
            }
            temp[i] = c;
        }
        return temp;
    }
    void writeString(const Interval& s, const std::string& t) {
        int length = s.length;
        assert(t.length() == length);
        for (int i = 0; i < length; i++) {
            Point p = s.getPoint(i);
            setCell(p, t[i]);
        }
    }
    bool nextPoint(Point& p, bool vertical) {
        if (vertical) {
            p.row++;
            if (p.row >= getRows()) {
                p.row = 0;
                p.col++;
            }
        }
        else {
            p.col++;
            if (p.col >= getCols()) {
                p.col = 0;
                p.row++;
            }
        }
        return isInside(p);
    }
    bool nextPointStopOnWrap(Point& p, bool vertical) {
        bool wrap = false;
        if (vertical) {
            p.row++;
            if (p.row >= getRows()) {
                p.row = 0;
                p.col++;
                wrap = true;
            }
        }
        else {
            p.col++;
            if (p.col >= getCols()) {
                p.col = 0;
                p.row++;
                wrap = true;
            }
        }
        return !wrap;
    }
    void fillIntervals(bool vertical) {
        Point p;
        while(isInside(p)) {
            while (isInside(p) && isBlock(p)) {
                nextPoint(p, vertical);
            }
            if (!isInside(p)) return;
            Point startPoint = p;

            int length = 0;
            bool continueLoop = false;
            do {
                continueLoop = nextPointStopOnWrap(p, vertical);
                length++;
            } while (continueLoop && !isBlock(p));
            intervals.push_back(Interval(startPoint, length, vertical));
        }
    }
    void fillIntervals() {
        assert(intervals.empty());
        fillIntervals(false);
        fillIntervals(true);
    }
    void loadFromFile(std::string fileName) {
        std::ifstream file;
        file.open(fileName);
        assert(file.is_open());
        while (!file.eof()) {
            std::string line;
            std::getline(file, line);
            if (!line.empty()) {
                int length = line.length();
                if (line[length-1] == '\r') {
                    line = line.substr(0, length-1);
                }
                rows_.push_back(line);
            }
        }
    }
    void verify() const {
        int rows = rows_.size();
        int cols = rows_[0].size();
        for (int i = 0; i < rows_.size(); i++) {
            assert(rows_[i].size() == cols);
        }
    }
    void print() const {
        std::cout << "Grid: " << name
             << " (rows=" << getRows()
             << ", cols=" << getCols() << ")"
             << " maxSize=" << getMaxSize() << std::endl;
        for (int i = 0; i < rows_.size(); i++) {
            std::string s = rows_[i];
            std::cout << s << "  (" << s.length() << ")" << std::endl;
        }
    }
    void printIntervals() const {
        std::cout << "Intervals:" << std::endl;
        for (const Interval& s : intervals) {
            Attribute attribute;
            std::cout << "  " << s << " " << getString(s, attribute) << std::endl;
        }
    }
    std::string name;
    std::vector<std::string> rows_;
    Intervals intervals;
};

struct Slot {
    Slot(const Interval s, const std::string& p) : interval(s), pattern(p) {}
    friend std::ostream& operator<<(std::ostream& os, const Slot& s);

    Interval interval;
    std::string pattern;
};
typedef std::vector<Slot> Slots;

std::ostream& operator<<(std::ostream& os, const Slot& s) {
    os << s.interval << " '" << s.pattern << "'";
    return os;
}

class Solver {
    public:
        Solver() {}
        void solve(const Grid& grid) {
            std::cout << "Solving... \n\n\n" << std::endl;
            loop(grid);
        }

    private:
        void loop(Grid grid) {
            Slots emptySlots;
            Slots partialSlots;
            Slots fullSlots;

            for (const Interval& s : grid.intervals) {
                Attribute attribute;
                std::string temp = grid.getString(s, attribute);
                if (attribute.isEmpty()) {
                    emptySlots.push_back(Slot(s, temp));
                } else if (attribute.isPartial()) {
                    partialSlots.push_back(Slot(s, temp));
                } else if (attribute.isFull()) {
                    fullSlots.push_back(Slot(s, temp));
                }
            }
            int numEmpty = emptySlots.size();
            int numPartial = partialSlots.size();
            int numFull = fullSlots.size();

            if (numPartial == 0 && numEmpty == 0) {
                std::cout << "SOLUTION!" << std::endl;
                grid.print();
                solutionFound = true;
                return;
            }
            assert(numPartial > 0);
            commitSlot(grid, partialSlots[0]);
        }
        void commitSlot(Grid& grid, const Slot& slot) {
            if(solutionFound == true)
                return;

            const Words* words = dictionary.findWord(slot.pattern);
            if (words) {
                for(const Word* w : *words) {
                    grid.writeString(slot.interval, w->word);
                    loop(grid);
                }
            }
            return;
        }
        bool solutionFound = false;
};

int main() {
    Grid grid("T1 - AI");

    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    std::cout << "Grid: loading" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    grid.loadFromFile("./resources/grid-7x7.txt");
    std::cout << "Grid: verifying" << std::endl;
    grid.verify();
    std::cout << "Grid: filling intervals" << std::endl;
    grid.fillIntervals();
    std::cout << std::endl;
    grid.print();

    std::cout << "Dictionary: loading" << std::endl;
    dictionary.parseFile("./resources/teste.txt", grid.getMaxSize());

    std::cout << "Solver: creating" << std::endl;
    Solver solver;
    solver.solve(grid);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\n\n\t***Game completed in " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]!***" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    return 0;
}