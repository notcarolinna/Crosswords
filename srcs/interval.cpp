#include "../include/interval.h"
#include <cassert>

Point Interval::getPoint(int i) const {
    assert(i >= 0 && i < length);
    if(vertical) {
        return Point(point.row+i, point.col);
    }
    else {
        return Point(point.row, point.col+i);
    }
}

std::ostream& operator<<(std::ostream& os, const Interval& s) {
    os << '[' << s.point << ']' << " length=" << s.length << " vertical=" << s.vertical << ')';
    return os;
}