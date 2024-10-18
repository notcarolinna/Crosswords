#include "./include/Solver.h"

std::ostream& operator<<(std::ostream& os, const Slot& s) {
    os << s.interval << " '" << s.pattern << "'";
    return os;
}

Solver::Solver(const Dictionary& dict) : dictionary(dict) {}

void Solver::solve(const Grid& grid) {
    std::cout << "Solving... \n\n\n" << std::endl;
    loop(grid);
}

void Solver::loop(Grid grid) {
    if (solutionFound)
        return;

    Slots emptySlots;
    Slots partialSlots;
    Slots fullSlots;
    Grid::Attribute attribute;

    for (const Grid::Interval& s : grid.getIntervals()) {
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

    //grid.print();

    if (numPartial == 0 && numEmpty == 0) {
        std::cout << "SOLUTION!" << std::endl;
        grid.print();
        solutionFound = true;
        return;
    }
    //assert(numPartial > 0 || numEmpty > 0);

    if (numPartial == 0 && numEmpty > 0) {
        //std::cout << partialSlots[0].pattern << std::endl;
        commitSlot(grid, emptySlots[0]);
    } else {
        //std::cout << emptySlots[0].pattern << std::endl;
        assert(numPartial > 0);
        commitSlot(grid, partialSlots[0]);
    }
}

void Solver::commitSlot(Grid& grid, const Slot& slot) {
    if (solutionFound == true)
        return;

    const Words* words = dictionary.findWord(slot.pattern);
    if (words) {
        for (const Word* w : *words) {
            grid.writeString(slot.interval, w->word);
            loop(grid);
        }
    }
    return;
}