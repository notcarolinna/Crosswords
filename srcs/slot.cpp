#include "../include/slot.h"

std::ostream& operator<<(std::ostream& os, const Slot& s) {
    os << s.interval << " '" << s.pattern << "'";
    return os;
}