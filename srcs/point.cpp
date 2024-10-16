#include "../include/point.h"

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << '(' << p.row << ',' << p.col << ')';
    return os;
}