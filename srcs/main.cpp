#include "Grid.h"
#include "Library.h"
#include "Solver.h"

int main() {
    Grid grid("MY GRID");
    grid.LoadFromFile("./resources/grid-teste.txt");
    grid.Check();
    grid.FillSpans();

    lib.ReadFromFile("./resources/teste2.txt", grid.max_size());

    Solver solver;
    solver.Solve(grid);

    return 0;
}