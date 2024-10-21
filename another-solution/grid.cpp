#include "grid.h"

#include <fstream>
#include <iostream>

#include <assert.h>

Grid::Grid(std::string name) :
    _name(name) {}

int Grid::getRowSize() const {
    return _grid.size();
}

int Grid::getColSize() const {
    return _grid.empty() ? 0 : _grid.front().size();
}

void Grid::loadFromFile(std::string fileName) {
    std::ifstream file;
    file.open(fileName);
    assert(file.is_open());

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        if (!line.empty()) {
            int length = line.length();
            if (line[length-1] == '\r') {
                line = line.substr(0, length-1);
            }
            std::vector<int> lineVector = {};
            for (char c : line) {
                lineVector.push_back(c);
            }
            _grid.push_back(lineVector);
            
        }
    }
}

void Grid::verify() const {
    for (int i = 0; i < getRowSize(); i++) {
        assert(_grid[i].size() == getColSize());
    }
}

bool Grid::isInside(const int i, const int j) const {
    return (i >= 0 && i < getRowSize() && j >= 0 && j < getColSize());
}

int Grid::getWordLength(int& i, int& j, const Slot::Direction& direction, const int wordId) {
    if (!isInside(i, j) || _grid[i][j] == '.') {
        return 0;
    }

    direction == Slot::Direction::HORIZONTAL ? j += 1 : i += 1;

    return 1 + getWordLength(i, j, direction, wordId);
}

void Grid::fillRestrictionHelper(Slot* slot) {
    assert(slot->getDirection() == Slot::Direction::HORIZONTAL);
    for(int i = 0; i < slot->getSize(); i++) {
        _grid[slot->getStartPos().first][slot->getStartPos().second + i] = slot->getId();
    }
}

std::vector<Restriction*> Grid::getRestrictions(Slot* slot, const std::vector<Slot*> slots) {
    std::vector<Restriction*> restrictions = {};
    assert(slot->getDirection() == Slot::Direction::VERTICAL);
    for(int i = 0; i < slot->getSize(); i++) {
        
        int idOtherWord = _grid[slot->getStartPos().first + i][slot->getStartPos().second];
        if ( idOtherWord != '?') {
            Restriction* restriction = new Restriction();
            restriction->setMyLetterPos(i);
            restriction->setIdOtherWord(idOtherWord);
            restriction->setOtherWordLetterPos(slot->getStartPos().second - slots[idOtherWord]->getStartPos().second);
            restrictions.push_back(restriction);
        }
    }

    return restrictions;
}

std::vector<Slot*> Grid::getSlots() {
    std::vector<Slot*> slots = {};
    for (int i = 0; i < getRowSize(); i++) {
        for (int j = 0; j < getColSize(); j++) {        
            std::pair<int, int> startPos = {i, j};
            int length = getWordLength(i, j, Slot::Direction::HORIZONTAL, slots.size());

            if (length > 1) {
                Slot* slot = new Slot();
                slot->setId(slots.size());
                slot->setDirection(Slot::Direction::HORIZONTAL);
                slot->setStartPos(startPos);
                slot->setSize(length);
                fillRestrictionHelper(slot);
                slots.push_back(slot);
            }
        }
    }

    for (int j = 0; j < getColSize(); j++) {
        for (int i = 0; i < getRowSize(); i++) {        
            std::pair<int, int> startPos = {i, j};
            int length = getWordLength(i, j, Slot::Direction::VERTICAL, slots.size());

            if (length > 1) {
                Slot* slot = new Slot();
                slot->setId(slots.size());
                slot->setDirection(Slot::Direction::VERTICAL);
                slot->setStartPos(startPos);
                slot->setSize(length);
                slot->setRestrictions(getRestrictions(slot, slots));
                slots.push_back(slot);

                for(Restriction* restriction : slot->getRestrictions()) {
                    Restriction* invertedRestriction = new Restriction();
                    invertedRestriction->setIdOtherWord(slot->getId());
                    invertedRestriction->setMyLetterPos(restriction->getOtherWordLetterPos());
                    invertedRestriction->setOtherWordLetterPos(restriction->getMyLetterPos());
                    slots[restriction->getIdOtherWord()]->addRestriction(invertedRestriction);
                }
            }
        }
    }
    return slots;
}

void Grid::showWords(const std::vector<Slot*>& slots) {
    for(Slot* slot : slots) {
        for(int i = 0; i < slot->getSize(); i++) {
            if (slot->getDirection() == Slot::Direction::HORIZONTAL) {
                _grid[slot->getStartPos().first][slot->getStartPos().second + i] = slot->getWord()[i];
            } else {
                _grid[slot->getStartPos().first + i][slot->getStartPos().second] = slot->getWord()[i];
            }
            
        }
    }
    print();
}

void Grid::print() const {
    std::cout << "Grid: " << _name
            << " (rows=" << getRowSize()
            << ", cols=" << getColSize() << ")" << std::endl << std::endl;
    for (int i = 0; i < getRowSize(); i++) {
        for (int j = 0; j < getColSize(); j++) {
            std::cout << ((char*)&_grid[i][j]);
        }
        std::cout << std::endl;

    }
}
