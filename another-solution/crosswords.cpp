#include <iostream>
#include <chrono>
#include <unordered_set>
#include <algorithm>

#include "grid.h"
#include "dictionary.h"
#include "solver.h"


int main() {
    Grid grid("T1 - AI");

    std::cout << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << std::endl;
    std::cout << "Grid: loading" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    grid.loadFromFile("../resources/grid-25x25.txt");
    std::cout << "Grid: verifying" << std::endl;
    grid.verify();
    grid.print();
    grid.loadSlots();

    std::unordered_set<int> wordsSize;
    std::vector<Slot*> slots = grid.getSlots();
    for (Slot* slot : slots) {
        std::cout << slot << std::endl << std::endl;
        wordsSize.insert(slot->getSize());
    }

    std::cout << "Dictionary: loading" << std::endl;
    Dictionary dictionary;
    dictionary.parseFile("../resources/palavras.txt", wordsSize);

    Solver solver = Solver(slots);

    for (int index : solver.getTreeIndexes()) {
        std::cout << index << ", ";
    }
    std::cout << std::endl;

    std::cout << "Solver: Start solving " << slots.size() << " slots" << std::endl;





    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\n\n\t***Game completed in " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]!***" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;    

    return 0;
}