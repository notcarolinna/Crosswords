#include <iostream>
#include "Span.h"

struct Slot {
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