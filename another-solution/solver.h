#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include <vector>

#include "slot.h"
#include "dictionary.h"

class Solver {

public:
    Solver(const std::vector<Slot*> slots);

    void loadTreeIndexes();
    std::vector<Slot*> getSlots() const;
    bool solve(const Dictionary* dictionary);

private:
    std::vector<int> _treeIndexes;
    std::vector<Slot*> _slots;

    int getMostRestricted();
    int getMostRestricted(std::vector<Restriction*> restrictions);

};

#endif // SOLVER_H