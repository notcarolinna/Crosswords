#include "Solver.h"
#include <iostream>
#include <cassert>

void Solver::Solve(const Grid& grid) {
    std::cout << "Solving... \n" << std::endl;
    grid.Print();
    Loop(grid, 0);
}

void Solver::Loop(Grid grid, int depth) {
    depth++;
    if (depth > 2) {
        std::cout << "Max depth reached" << std::endl;
        return;
    }
    Slots empty_slots;
    Slots partial_slots;
    Slots done_slots;
    for (const Span& s : grid.spans) {
        Attr attr;
        std::string temp = grid.GetString(s, attr);
        if (attr.is_empty()) {
            empty_slots.push_back(Slot(s, temp));
        } else if (attr.is_partial()) {
            partial_slots.push_back(Slot(s, temp));
        } else if (attr.is_full()) {
            done_slots.push_back(Slot(s, temp));
        }
    }

    int num_empty = empty_slots.size();
    int num_partial = partial_slots.size();
    int num_done = done_slots.size();
    std::cout << "Empty: " << num_empty << " Partial: " << num_partial << " Done: " << num_done << std::endl;

    for (const Slot& s : done_slots) {
        std::cout << "Checking " << s.pattern << std::endl;
        if (!lib.IsWord(s.pattern)) {
            std::cout << "No words found for " << s.pattern << std::endl;
            return;
        }
    }

    if (num_partial == 0 && num_empty == 0) {
        std::cout << "Done!" << std::endl;
        grid.Print();
        return;
    }

    assert(num_partial > 0);
    CommitSlot(grid, partial_slots[0], depth);
}

void Solver::CommitSlot(Grid& grid, const Slot& s, int depth) {
    std::cout << "Committing " << s.span << std::endl;
    std::cout << "Possible words: ";

    const Words* words = lib.FindWord(s.pattern);
    if (words) {
        for (const Word* w : *words) {
            std::cout << "\n";
            std::cout << "Committing ' " << w->word << std::endl;
            grid.WriteString(s.span, w->word);

            std::cout << "Grid after commit: " << std::endl;
            grid.Print();
            Loop(grid, depth);
        }
    } else {
        std::cout << "None" << std::endl;
    }
}