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
        const Words* FindWord(const string& s) const {
            auto it = word_map_.find(s);
            if (it != word_map_.end()) {
                return &it->second;
            } else {
                return NULL;
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
        void Solve(const Grid& grid) {
            cout << "Solving" << endl;
            grid.Print();
            Loop(grid);
        }

    private:
        void Loop(Grid grid) {
            Slots empty_slots;
            Slots partial_slots; // os que vamos usar
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

            //cout << "empty = " << num_empty << endl;
            //cout << "partial = " << num_partial << endl;
            //cout << "full = " << num_full << endl;

            for(const Slot& s : full_slots) {
                //cout << "CHECKING " << s.pattern << "if it is a word" << endl;
                if(!lib.IsWord(s.pattern)) {
                    //cout << "ABORT" << endl;
                    //return;
                }
            }

            if (num_partial == 0 && num_empty == 0) {
                cout << "SOLUTION!" << endl;
                grid.Print();
                return;
                // now do what
            }
            //if(first_exec) {
              //  first_exec = false;
               // CommitSlot(empty_slots[0]);
            //}
            //else {
                assert(num_partial > 0);
                CommitSlot(grid, partial_slots[0]);
           // }
        }
        void CommitSlot(Grid& grid, const Slot& slot) {
            //cout << "COMMIT slot" << slot << endl;
            //cout << "Possible word choices for this slot are" << endl;

            const Words* words = lib.FindWord(slot.pattern);
            if (words) {
                for(const Word* w : *words) {
                    //cout << "committing" << endl;
                    grid.WriteString(slot.span, w->word);
                    //cout << "new grid is:" << endl;
                    //grid.Print();
                    Loop(grid);
                }
            }
            else {
                //cout << "No matches to pattern" << endl;
            }

        }
        bool first_exec = true;
};

int main() {
    Grid grid("T1 - IA");
    grid.LoadFromFile("./resources/grid-teste.txt");
    grid.Check();
    grid.FillSpans();
    //grid.PrintSpans();

    lib.ReadFromFile("./resources/teste2.txt", grid.max_size());

    Solver solver;
    solver.Solve(grid);

    // ...

    return 0;
}
