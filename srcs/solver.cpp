#include "solver.h"
#include "grid.h"
#include <iostream>
#include <cassert>

void Solver::solve(const Grid& grid) {
    std::cout << "Solving... \n\n\n" << std::endl;
    loop(grid);
}

void Solver::loop(Grid grid) {
    if(solutionFound == true)
        return;

    Slots emptySlots;
    Slots partialSlots;
    Slots fullSlots;

    for (const Interval& s : grid.getIntervals()) {
        Attribute attribute;
        std::string temp = grid.getString(s, attribute);
        if (attribute.isEmpty()) {
            emptySlots.push_back(Slot(s, temp));
        } else if (attribute.isPartial()) {
            partialSlots.push_back(Slot(s, temp));
        } else if (attribute.isFull()) {
            fullSlots.push_back(Slot(s, temp));
        }
    }
    int numEmpty = emptySlots.size();
    int numPartial = partialSlots.size();
    int numFull = fullSlots.size();

    if (numPartial == 0 && numEmpty == 0) {
        std::cout << "SOLUTION!" << std::endl;
        grid.print();
        solutionFound = true;
        return;
    }
    assert(numPartial > 0);
    commitSlot(grid, partialSlots[0]);
}

void Solver::commitSlot(Grid& grid, const Slot& slot) {
    if(solutionFound == true)
        return;

    const Words* words = dictionary.findWord(slot.pattern);
    if (words) {
        for(const Word* w : *words) {
            grid.writeString(slot.interval, w->word);
            loop(grid);
        }
    }
    return;
}