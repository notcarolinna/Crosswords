#pragma once
#include "Dictionary.h"
#include "Grid.h"

struct Slot {
    Slot(const Grid::Interval s, const std::string& p) : interval(s), pattern(p) {}
    friend std::ostream& operator<<(std::ostream& os, const Slot& s);

    Grid::Interval interval;
    std::string pattern;
};
typedef std::vector<Slot> Slots;

class Solver {
public:
    Solver();
    Solver(const Dictionary& dict);
    void solve(const Grid& grid);

private:
    void loop(Grid grid);
    void commitSlot(Grid& grid, const Slot& slot);
    bool solutionFound = false;
    const Dictionary& dictionary; // Adiciona o membro dictionary
};