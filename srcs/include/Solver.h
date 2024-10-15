#ifndef SOLVER_H
#define SOLVER_H

#include "Grid.h"
#include "Library.h"
#include "Slot.h"

class Solver {
public:
    Solver() {}
    void Solve(const Grid& grid);

private:
    void Loop(Grid grid, int depth);
    void CommitSlot(Grid& grid, const Slot& s, int depth);
};

#endif // SOLVER_H