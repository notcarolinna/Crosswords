#include <iostream>
#include <vector>
#include "Point.h"
#include <cassert>

struct Span {
    Span(Point p, int l, bool v) : point(p), len(l), vert(v) {}

    Point GetPoint(int i) const;
    friend std::ostream& operator<<(std::ostream& os, const Span& s);

    Point point;
    int len;
    bool vert;
};

typedef std::vector<Span> Spans;

std::ostream& operator<<(std::ostream& os, const Span& s);

Point Span::GetPoint(int i) const {
    assert(i >= 0 && i < len);
    if (vert) {
        return Point(point.row + i, point.col);
    } else {
        return Point(point.row, point.col + i);
    }
}

std::ostream& operator<<(std::ostream& os, const Span& s) {
    os << "Span: " << s.point << " " << s.len << " " << s.vert;
    return os;
}