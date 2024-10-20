#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include "slot.h"

class Grid {

public:
    Grid(std::string name);
    
    int getRowSize() const;
    int getColSize() const;
    std::vector<Slot*> getSlots();
    void loadFromFile(std::string fileName);
    void verify() const;
    void showWords(const std::vector<Slot*>& slots);

    void print() const;

private:
    std::string _name;
    std::vector<std::vector<int>> _grid;

    bool isInside(const int i, const int j) const;
    int getWordLength(int& i, int& j, const Slot::Direction& direction, const int wordId);
    std::vector<Restriction*> getRestrictions(Slot* slot, const std::vector<Slot*> slots);
    void fillRestrictionHelper(Slot* slot);
};

#endif // GRID_H