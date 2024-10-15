#include "Grid.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

Grid::Grid() {
    name = "UNTITLED";
}

Grid::Grid(std::string n) {
    name = n;
}

int Grid::rows() const {
    return lines.size();
}

int Grid::cols() const {
    if (lines.empty()) {
        return 0;
    } else {
        return lines[0].size();
    }
}

int Grid::max_size() const {
    return std::max(rows(), cols());
}

char Grid::box(const Point& p) const {
    assert(in_bounds(p));
    return lines[p.row][p.col];
}

void Grid::write_box(const Point& p, char c) {
    assert(in_bounds(p));
    lines[p.row][p.col] = c;
}

bool Grid::is_block(const Point& p) const {
    return box(p) == '.';
}

bool Grid::is_blank(const Point& p) const {
    return box(p) == '?';
}

bool Grid::is_letter(const Point& p) const {
    char c = box(p);
    return c >= 'A' && c <= 'Z';
}

bool Grid::in_bounds(const Point& p) const {
    return p.row >= 0 && p.row < rows() && p.col >= 0 && p.col < cols();
}

std::string Grid::GetString(const Span& s, Attr& attr) const {
    int len = s.len;
    std::string temp;
    temp.resize(len);
    for (int i = 0; i < len; i++) {
        Point p = s.GetPoint(i);
        char c = box(p);
        if (c == '?') {
            attr.has_blanks = true;
        } else if (c >= 'A' && c <= 'Z') {
            attr.has_letters = true;
        }
        temp[i] = c;
    }
    return temp;
}

void Grid::WriteString(const Span& s, const std::string& t) {
    int len = s.len;
    assert(t.length() == len);
    for (int i = 0; i < len; i++) {
        Point p = s.GetPoint(i);
        write_box(p, t[i]);
    }
}

bool Grid::Next(Point& p, bool vert) {
    if (vert) {
        p.row++;
        if (p.row >= rows()) {
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
    return in_bounds(p);
}

bool Grid::NextStopAtWrap(Point& p, bool vert) {
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

void Grid::FillSpans(bool vert) {
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

void Grid::FillSpans() {
    assert(spans.empty());
    FillSpans(false);
    FillSpans(true);
}

int Grid::LoadFromFile(std::string fileName) {
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

int Grid::Check() const {
    int rows = lines.size();
    int cols = lines[0].size();
    for (int i = 3; i < lines.size(); i++) {
        assert(lines[i].size() == cols);
    }
    return 0;
}

int Grid::Print() const {
    std::cout << "Grid: " << name
              << " (rows=" << rows()
              << ", cols=" << cols() << ")\n"
              << " max_size=" << max_size() << std::endl;
    for (std::string s : lines) {
        std::cout << "   " << s << std::endl;
    }
    return 0;
}

void Grid::PrintSpans() const {
    std::cout << "Spans:\n";
    for (const Span& s : spans) {
        Attr attr;
        std::cout << "  " << s << " " << GetString(s, attr) << std::endl;
    }
}