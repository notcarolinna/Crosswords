#include <iostream>

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

