#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <algorithm>
using namespace std;

// -------------------------------------------------------------------------
//-Point
struct Point {
    Point() {}
    Point(int r, int c) : row(r), col(c) {}

    friend ostream& operator<<(ostream& os, const Point& p);
    int row = 0;
    int col = 0;
};

ostream& operator<<(ostream& os, const Point& p) {
    os << '(' << p.row << ',' << p.col << ')';
    return os;
}

// -------------------------------------------------------------------------
//-Span
struct Span {
    Span(Point p, int l, bool v) : point(p), len(l), vert(v) {}
    Point GetPoint(int i) const {
        assert(i >= 0 && i < len);
        if(vert) {
            return Point(point.row+i, point.col);
        }
        else {
            return Point(point.row, point.col+i);
        }
    }
    friend ostream& operator<<(ostream& os, const Span& s);

    Point point;
    int len;
    bool vert;
};
typedef vector<Span> Spans;

ostream& operator<<(ostream& os, const Span& s) {
    os << '[' << s.point << ']' << "len=" << s.len << " vert=" << s.vert << ')';
    return os;
}

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
            if (len > 7) return;
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
        void ReadFromFile(string fileName, int max_size) {
            ifstream file;
            file.open(fileName);
            while (file.is_open() && !file.eof()) {
                string line;
                getline(file, line);
                if (!line.empty()) {
                    int len = line.length();
                    if (line[len-1] == '\r') {
                        line = line.substr(0, len-1);
                    }
                    len = line.length();
                    if(all_of(line.begin(), line.end(), [] (char c) {return isalpha(c);} )) {
                        if (len <= max_size) {
                            Word* w = new Word(line);
                            words_.push_back(w);
                            CreatePatternHash(w);
                        }
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

Library lib;

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
    int max_size() const { return max(rows(), cols()); }
    char box(const Point& p) const {
        assert(in_bounds(p));
        return lines[p.row][p.col];
    }
    bool is_block(const Point& p) const {
        return box(p) == '.';
    }
    bool is_blank(const Point& p) const {
        return box(p) == '?';
    }
    bool is_letter(const Point& p) const {
        char c = box(p);
        return c >= 'A' && c <= 'Z';
    }
    bool in_bounds(const Point& p) const {
        return (p.row >= 0 && p.row < rows() && p.col >= 0 && p.col < cols());
    }
    string GetString(const Span& s) const {
        int len = s.len;
        string temp;
        temp.resize(len);
        for (int i = 0; i < len; i++) {
            Point p = s.GetPoint(i);
            temp[i] = box(p);
        }
        return temp;
    }
    bool Next(Point& p, bool vert) {
        if (vert) {
            p.row++;
            if (p.row >= rows()) {
                p.row = 0;
                p.col++;
            }
        }
        else {
            p.col++;
            if (p.col >= cols()) {
                p.col = 0;
                p.row++;
            }
        }
        return in_bounds(p);
    }
    bool NextStopAtWrap(Point& p, bool vert) {
        bool wrap = false;
        if (vert) {
            p.row++;
            if (p.row >= rows()) {
                p.row = 0;
                p.col++;
                wrap = true;
            }
        }
        else {
            p.col++;
            if (p.col >= cols()) {
                p.col = 0;
                p.row++;
                wrap = true;
            }
        }
        return !wrap;
    }
    void FillSpans(bool vert) {
        Point p;
        while(in_bounds(p)) {
            while (in_bounds(p) && is_block(p)) {
                Next(p, vert);
            }
            if (!in_bounds(p)) return;
            Point startp = p;
            //cout << "SPAN START: " << p <<endl;

            int len = 0;
            bool keep_going = false;
            do {
                keep_going = NextStopAtWrap(p, vert);
                len++;
            } while (keep_going && !is_block(p));
            //cout << "END OF SPAN!!! len=" << len << endl;
            spans.push_back(Span(startp, len, vert));
        }
    }
    void FillSpans() {
        assert(spans.empty());
        FillSpans(false); // horizontal
        FillSpans(true); // vertical
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
             << ", cols=" << cols() << ")"
             << " max_size=" << max_size() << endl;
        for (string s : lines) {
            cout << "   " << s << endl;
        }
    }
    void PrintSpans() const {
        cout << "Spans:" << endl;
        for (const Span& s : spans) {
            cout << "  " << s << " " << GetString(s) << endl;
        }
    }
    string name;
    vector<string> lines;
    Spans spans;

};

// -------------------------------------------------------------------------
//-Solver
class Solver {
    public:
        Solver(Grid* g) : grid_(g) {}
        void Solve() {
            cout << "Solving" << endl;
            grid_->Print();
            Loop();
        }

    private:
        void Loop() {
            for (const Span& s : grid_->spans) {

            }
        }
        Grid* grid_;
};

int main() {
    Grid grid("T1 - IA");
    grid.LoadFromFile("./resources/grid-teste-2.txt");
    grid.Check();
    grid.Print();
    grid.FillSpans();
    //grid.PrintSpans();

    lib.ReadFromFile("./resources/teste2.txt", grid.max_size());

    //Solver solver(grid);
    //solver.Solve();

    // ...

    return 0;
}