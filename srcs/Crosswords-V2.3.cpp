#include "./include/grid.h"
#include "./include/dictionary.h"
#include "./include/solver.h"
#include <chrono>
#include <iostream>


int main() {
    Grid grid("T1 - AI");

    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    std::cout << "Grid: loading" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    grid.loadFromFile("./resources/grid-7x7.txt");
    std::cout << "Grid: verifying" << std::endl;
    grid.verify();
    std::cout << "Grid: filling intervals" << std::endl;
    grid.fillIntervals();
    std::cout << std::endl;
    grid.print();

    std::cout << "Dictionary: loading" << std::endl;
    dictionary.parseFile("./resources/palavras.txt", grid.getMaxSize());

    std::cout << "Solver: creating" << std::endl;
    Solver solver;
    solver.solve(grid);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\n\n\t***Game completed in " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]!***" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    return 0;
}