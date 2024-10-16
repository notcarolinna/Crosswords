#ifndef SLOT_H
#define SLOT_H

#include "interval.h"
#include <string>
#include <vector>
#include <iostream>

struct Slot {
    Slot(const Interval s, const std::string& p) : interval(s), pattern(p) {}
    friend std::ostream& operator<<(std::ostream& os, const Slot& s);

    Interval interval;
    std::string pattern;
};

typedef std::vector<Slot> Slots;

std::ostream& operator<<(std::ostream& os, const Slot& s);

#endif // SLOT_H