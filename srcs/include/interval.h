#ifndef INTERVAL_H
#define INTERVAL_H

#include "point.h"
#include <vector>
#include <iostream>

struct Interval {
    Interval(Point p, int l, bool v) : point(p), length(l), vertical(v) {}
    Point getPoint(int i) const;

    friend std::ostream& operator<<(std::ostream& os, const Interval& s);

    Point point;
    int length;
    bool vertical;
};

typedef std::vector<Interval> Intervals;

std::ostream& operator<<(std::ostream& os, const Interval& s);

#endif // INTERVAL_H