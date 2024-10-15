#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>
#include "Point.h"
#include "Span.h"
#include "Word.h"

struct Attr {
    bool is_empty() const { return has_blanks && !has_letters; }
    bool is_partial() const { return has_blanks && has_letters; }
    bool is_full() const { return !has_blanks && has_letters; }
    bool has_letters = false;
    bool has_blanks = false;
};

struct Grid {
    Grid();
    Grid(std::string n);
    int rows() const;
    int cols() const;
    int max_size() const;
    char box(const Point& p) const;
    void write_box(const Point& p, char c);
    bool is_block(const Point& p) const;
    bool is_blank(const Point& p) const;
    bool is_letter(const Point& p) const;
    bool in_bounds(const Point& p) const;
    std::string GetString(const Span& s, Attr& attr) const;
    void WriteString(const Span& s, const std::string& t);
    bool Next(Point& p, bool vert);
    bool NextStopAtWrap(Point& p, bool vert);
    void FillSpans(bool vert);
    void FillSpans();
    int LoadFromFile(std::string fileName);
    int Check() const;
    int Print() const;
    void PrintSpans() const;

    std::string name;
    std::vector<std::string> lines;
    Spans spans;
};

#endif // GRID_H