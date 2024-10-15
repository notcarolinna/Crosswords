#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>

struct Point {
    Point() {}
    Point(int r, int c) : row(r), col(c) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    int row = 0;
    int col = 0;
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.row << ", " << p.col << ")";
    return os;
}

struct Span {
    Span(Point p, int l, bool v) : point(p), len(l), vert(v) {}

    Point GetPoint(int i) const{
        assert(i >= 0 && i < len);
        if (vert) {
            return Point(point.row + i, point.col);
        } else {
            return Point(point.row, point.col + i);
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Span& s);

    Point point;
    int len;
    bool vert;
};

typedef std::vector<Span> Spans;

std::ostream& operator<<(std::ostream& os, const Span& s) {
    os << "Span: " << s.point << " " << s.len << " " << s.vert;
    return os;
}

struct Word {
    Word() {}
    Word(std::string s) : word(s) {}
    int len() const { return word.length(); }
    std::string word;
};
typedef std::vector<Word*> Words;
typedef std::unordered_map<std::string, Words> WordMap;


class Library {
public:
    Library() {}
    ~Library() {
        for (Word* w : words_) {
            delete w;
        }
    }

    const Words* FindWord(const std::string& s) const {
        auto it = word_map_.find(s);

        if (it != word_map_.end()) {
            return &it->second;
        } else {
            return NULL;
        }
    }

    bool IsWord(std::string s) const {
        auto it = word_map_.find(s);
        if (it == word_map_.end()) {
            return false;
        } else {
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
        std::cout << "counts of each word length" << std::endl;
        for (int i = 1; i < counts_.size(); i++) {
            std::cout << i << ") " << counts_[i] << std::endl;
        }
    }
    std::string GetWord(int i) const {
        assert(i >= 0 && i < words_.size());
        return words_[i]->word;
    }
    void CreatePatternHash(Word* w) {
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

    void ReadFromFile(std::string fileName, int max_size) {
        std::ifstream file;
        file.open(fileName);
        while (file.is_open() && !file.eof()) {
            std::string line;
            std::getline(file, line);

            if (!line.empty()) {
                int len = line.length();
                if (std::all_of(line.begin(), line.end(), [](char c) { return std::isalpha(c); })) {
                    if(line.length() <= max_size){
                        Word* w = new Word(line);
                        words_.push_back(w);
                        CreatePatternHash(w);
                    }
                }
            }
        }
        std::cout << "Read " << words_.size() << " words" << std::endl;
    }

    void DebugBuckets() const {
        for (int i = 0; i < word_map_.bucket_count(); i++) {
            std::cout << "[" << i << "] " << word_map_.bucket_size(i) << std::endl;
        }
    }
private:
    Words words_;
    WordMap word_map_;
    std::vector<int> counts_;
};

Library lib;

struct Attr{
    bool is_empty() const{ return has_blanks && !has_letters; }
    bool is_partial() const{ return has_blanks && has_letters; }
    bool is_full() const{ return !has_blanks && has_letters; }
    bool has_letters = false;
    bool has_blanks = false;
};

struct Grid {
    
    Grid() {
        name = "UNTITLED";
    }

    Grid(std::string n) {
        name = n;
    }
    int rows() const { return lines.size(); }
    int cols() const {
        if (lines.empty()) {
            return 0;
        } else {
            return lines[0].size();
        }
    }

    int max_size() const { return std::max(rows(), cols()); }

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
        return p.row >= 0 && p.row < rows() && p.col >= 0 && p.col < cols();
    }

    std::string GetString(const Span& s, Attr& attr) const{
        int len = s.len;
        std::string temp;
        temp.resize(len);
        for(int i = 0; i < len; i++){
            Point p = s.GetPoint(i);
            char c = box(p);
            if(c == '?'){
                attr.has_blanks = true;
            } else if (c >= 'A' && c <= 'Z'){
                attr.has_letters = true;
            }
            temp[i] = c;
        }
        return temp;
    }

    void WriteString(const Span& s, const std::string& t) {
        int len = s.len;
        assert(t.length() == len);
        for(int i = 0; i < len; i++){
            Point p = s.GetPoint(i);
            write_box(p, t[i]);
        }
    }

    bool Next(Point& p, bool vert) {
        if (vert) {
            p.row++;
        if(p.row >= rows()){
            p.row = 0;
            p.col++;
        }
        } else {
            p.col++;
            if (p.col >= cols()) {
                p.col = 0;
                p.row++;
            }
        }
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
        } else {
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
        while (in_bounds(p)) {
            while (in_bounds(p) && is_block(p)) {
                Next(p, vert);
            }
            if (!in_bounds(p)) {
                return;
            }
            Point start = p;
            std::cout << "Start: " << start << std::endl;

            int len = 0;
            bool keep_going = false;
            do {
               keep_going = NextStopAtWrap(p, vert);
                len++;
            } while (in_bounds(p) && !is_block(p));

            std::cout << "Span: " << len << std::endl;
            spans.push_back(Span(start, len, vert));
        }
    }

    void FillSpans() {
        assert(spans.empty());
        FillSpans(false);
        FillSpans(true);
    }

    int LoadFromFile(std::string fileName) {
        std::ifstream file;
        file.open(fileName);
        while (file.is_open() && !file.eof()) {
            std::string line;
            std::getline(file, line);
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        return 0;
    }

    int Check() const {
        int rows = lines.size();
        int cols = lines[0].size();
        for (int i = 3; i < lines.size(); i++) {
            assert(lines[i].size() == cols);
        }
        return 0;
    }

    int Print() const {
        std::cout << "Grid: " << name
                  << " (rows=" << rows()
                  << ", cols=" << cols() << ")\n"
                  << " max_size=" << max_size() << std::endl;
        for (std::string s : lines) {
            std::cout << "   " << s << std::endl;
        }
        return 0;
    }

    void PrintSpans() const {
        std::cout << "Spans:\n";
        for (const Span& s : spans) {
            Attr attr;
            std::cout << "  " << s << " " << GetString(s, attr) << std::endl;
        }
    }

    std::string name;
    std::vector<std::string> lines;
    Spans spans;
};

struct Slot{
    Slot(const Span& s, const std::string& p) : span(s), pattern(p) {}
    friend std::ostream& operator<<(std::ostream& os, const Slot& s);

    Span span;
    std::string pattern;
};

typedef std::vector<Slot> Slots;

std::ostream& operator<<(std::ostream& os, const Slot& s) {
    os << s.span << " " << s.pattern;
    return os;
}

class Solver{
    public:
    Solver() {}
    void Solve(const Grid& grid){
        std::cout << "Solving... \n" << std::endl;
        grid.Print();
        Loop(grid, 0);
    }
    private:
    void Loop (Grid grid, int depth) {
        depth++;
        if(depth > 2){
            std::cout << "Max depth reached" << std::endl;
            return;
        }
        Slots empty_slots;
        Slots partial_slots;
        Slots done_slots;
        for(const Span& s  : grid.spans){
            Attr attr;
            std::string temp = grid.GetString(s, attr);
            if(attr.is_empty()){
                empty_slots.push_back(Slot(s, temp));
            } else if(attr.is_partial()){
                partial_slots.push_back(Slot(s, temp));
            } else if(attr.is_full()){
                done_slots.push_back(Slot(s, temp));
            }
        }

        int num_empty = empty_slots.size();
        int num_partial = partial_slots.size();
        int num_done = done_slots.size();
        std::cout << "Empty: " << num_empty << " Partial: " << num_partial << " Done: " << num_done << std::endl;

        for(const Slot& s : done_slots){
            std::cout << "Checking " << s.pattern << std::endl;
            if(!lib.IsWord(s.pattern)){
                std::cout << "No words found for " << s.pattern << std::endl;
                return;
            }
        }

        if(num_partial == 0 && num_empty == 0){
            std::cout << "Done!" << std::endl;
            grid.Print();
            return;
        }

        assert(num_partial > 0);
        CommitSlot(grid, partial_slots[0], depth);
    }

    void CommitSlot(Grid& grid, const Slot& s, int depth){
        std::cout << "Committing " << s.span << std::endl;
        std::cout << "Possible words: ";

        const Words* words = lib.FindWord(s.pattern);
        if(words){
            for (const Word* w : *words){
            std::cout << "\n";
            std::cout << "Commiting ' " << w->word << std::endl;
            grid.WriteString(s.span, w->word);

            std::cout << "Grid after commit: " << std::endl;
            grid.Print();
            Loop(grid, depth);
            }
        } else {
            std::cout << "None" << std::endl;
        }
    }
};

int main() {

    Grid grid("MY GRID");
    grid.LoadFromFile("./resources/grid-teste.txt");
    grid.Check();
    grid.FillSpans();

    lib.ReadFromFile("./resources/teste2.txt", grid.max_size());

    Solver solver;
    solver.Solve(grid);

    return 0;
}