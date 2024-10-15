#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
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
    os << '[' << s.point << ']' << " len=" << s.len << " vert=" << s.vert;
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
        const Words* FindWord(const string& s) const {
            auto it = word_map_.find(s);
            if (it != word_map_.end()) {
                return &it->second;
            } else {
                return nullptr;
            }
        }
        bool IsWord(string s) const {
            auto it = word_map_.find(s);
            return it != word_map_.end();
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
            for (size_t i = 1; i < counts_.size(); i++) {
                cout << i << ") " << counts_[i] << endl;
            }
        }
        string GetWord(size_t i) const {
            assert(i < words_.size());
            return words_[i]->word;
        }
        void CreatePatternHash(Word* w) {
            int len = w->len();
            if (len > 7) return;
            int num_patterns = 1 << len;
            for (int i = 0; i < num_patterns; i++) {
                string temp = w->word;
                for (int j = 0; j < len; j++) {
                    if ((i >> j) & 1) {
                        temp[j] = '?';
                    }
                }
                word_map_[temp].push_back(w);
            }
        }
        void ReadFromFile(string fileName, int max_size) {
            ifstream file(fileName);
            if (!file.is_open()) {
                cerr << "Error opening file: " << fileName << endl;
                return;
            }
            while (!file.eof()) {
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
            for (size_t i = 0; i < word_map_.bucket_count(); i++) {
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
//-Attr
struct Attr {
    bool is_empty() const { return has_blanks && !has_letters; }
    bool is_partial() const { return has_blanks && has_letters; }
    bool is_full() const { return !has_blanks && has_letters; }
    bool has_letters = false;
    bool has_blanks = false;
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
    int max_size() const { return max(rows(), cols()); }
    char box(const Point& p) const {
        assert(in_bounds(p));
        return lines[p.row][p.col];
    }
    void write_box(const Point& p, char c) {
        assert(in_bounds(p));
        lines[p.row][p.col] = c;
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
    string GetString(const Span& s, Attr& attr) const {
        int len = s.len;
        string temp;
        temp.resize(len);
        for (int i = 0; i < len; i++) {
            Point p = s.GetPoint(i);
            char c = box(p);
            if (c == '?') {
                attr.has_blanks = true;
            }
            else if (c >= 'A' && c <= 'Z') {
                attr.has_letters = true;
            }
            temp[i] = c;
        }
        return temp;
    }
    void WriteString(const Span& s, const string& t) {
        int len = s.len;
        assert(t.length() == len);
        for (int i = 0; i < len; i++) {
            Point p = s.GetPoint(i);
            write_box(p, t[i]);
        }
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

            int len = 0;
            bool keep_going = false;
            do {
                keep_going = NextStopAtWrap(p, vert);
                len++;
            } while (keep_going && !is_block(p));
            spans.push_back(Span(startp, len, vert));
        }
    }
    void FillSpans() {
        assert(spans.empty());
        FillSpans(false); // horizontal
        FillSpans(true); // vertical
    }
    void LoadFromFile(string fileName) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }
        while (!file.eof()) {
            string line;
            getline(file, line);
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
    }
    void Check() const {
        int rows = lines.size();
        int cols = lines[0].size();
        for (int i = 0; i < rows; i++) {
            assert(lines[i].size() == cols);
        }
    }
    void Print() const {
        cout << "Grid: " << name
             << " (rows=" << rows()
             << ", cols=" << cols() << ")"
             << " max_size=" << max_size() << endl;
        for (const string& s : lines) {
            cout << "   " << s << endl;
        }
    }
    void PrintSpans() const {
        cout << "Spans:" << endl;
        for (const Span& s : spans) {
            Attr attr;
            cout << "  " << s << " " << GetString(s, attr) << endl;
        }
    }
    string name;
    vector<string> lines;
    Spans spans;

};

// -------------------------------------------------------------------------
//-Slot
struct Slot {
    Slot(const Span s, const string& p) : span(s), pattern(p) {}
    friend ostream& operator<<(ostream& os, const Slot& s);

    Span span;
    string pattern;
};
typedef vector<Slot> Slots;

ostream& operator<<(ostream& os, const Slot& s) {
    os << s.span << " '" << s.pattern << "'";
    return os;
}

// -------------------------------------------------------------------------
//-Solver
class Solver {
    public:
        Solver() {}
        void Solve(Grid& grid) {
            cout << "Solving" << endl;
            grid.Print();
            Loop(grid);
        }

    private:
        void Loop(Grid& grid) {
            Slots empty_slots;
            Slots partial_slots;
            Slots full_slots;

            for (const Span& s : grid.spans) {
                Attr attr;
                string temp = grid.GetString(s, attr);
                if (attr.is_empty()) {
                    empty_slots.push_back(Slot(s, temp));
                } else if (attr.is_partial()) {
                    partial_slots.push_back(Slot(s, temp));
                } else if (attr.is_full()) {
                    full_slots.push_back(Slot(s, temp));
                }
            }
            int num_empty = empty_slots.size();
            int num_partial = partial_slots.size();
            int num_full = full_slots.size();

            for(const Slot& s : full_slots) {
                if(!lib.IsWord(s.pattern)) {
                    cerr << "Error: Full slot pattern is not a valid word: " << s.pattern << endl;
                    return;
                }
            }

            if (num_partial == 0 && num_empty == 0) {
                cout << "SOLUTION!" << endl;
                grid.Print();
                return;
            }

            if (num_partial > 0) {
                CommitSlot(grid, partial_slots[0]);
            } else if (num_empty > 0) {
                CommitSlot(grid, empty_slots[0]);
            }
        }
        void CommitSlot(Grid& grid, const Slot& slot) {
            const Words* words = lib.FindWord(slot.pattern);
            if (words) {
                for(const Word* w : *words) {
                    if (w->len() == slot.span.len) { // Verifica se a palavra cabe no espaço
                        grid.WriteString(slot.span, w->word);
                        Loop(grid);
                    }
                }
            } else {
                cerr << "No matches to pattern: " << slot.pattern << endl;
            }
        }
};

int main() {
    Grid grid("T1 - IA");
    grid.LoadFromFile("./resources/grid-teste.txt");
    grid.Check();
    grid.FillSpans();

    lib.ReadFromFile("./resources/teste2.txt", grid.max_size());

    Solver solver;
    solver.Solve(grid);

    return 0;
}