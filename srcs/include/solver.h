#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "slot.h"
#include "dictionary.h"

class Solver {
    public:
        Solver() {}
        void solve(const Grid& grid);

    private:
        void loop(Grid grid);
        void commitSlot(Grid& grid, const Slot& slot);
        bool solutionFound = false;
};

#endif // SOLVER_H