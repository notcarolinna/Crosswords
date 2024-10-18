#include "./include/Crosswords.h"
#include "./include/Grid.h"
#include "./include/Dictionary.h"
#include "./include/Solver.h"

int main() {
    Dictionary dictionary;
    Solver solver(dictionary);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Grid grid("T1 - AI");

    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    std::cout << "Grid: loading" << std::endl;

    loadAndVerifyGrid(grid, "./resources/grid-7x7.txt");
    std::cout << "Dictionary: loading" << std::endl;
    loadDictionary(dictionary, "./resources/palavras.txt", grid.getMaxSize());

    std::cout << "Solver: creating" << std::endl;
    solveGrid(solver, grid);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    printCompletionTime(begin, end);

    return 0;
}

void loadAndVerifyGrid(Grid& grid, const std::string& filename) {
    grid.loadFromFile(filename);
    std::cout << "Grid: verifying" << std::endl;
    grid.verify();
    std::cout << "Grid: filling intervals" << std::endl;
    grid.fillIntervals();
    std::cout << std::endl;
    grid.print();
}

void loadDictionary(Dictionary& dictionary, const std::string& filename, int maxSize) {
    dictionary.parseFile(filename, maxSize);
}

void solveGrid(Solver& solver, Grid& grid) {
    solver.solve(grid);
}

void printCompletionTime(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end) {
    std::cout << "\n\n\t***Game completed in " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]!***" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}