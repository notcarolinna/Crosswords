#include "solver.h"

#include <algorithm>

Solver::Solver(const std::vector<Slot*> slots) :
    _slots(slots),
    _treeIndexes({}) {
    loadTreeIndexes();
}

void Solver::loadTreeIndexes() {
    _treeIndexes = {};
    
    int indexToLook = -1;
    while(_treeIndexes.size() < _slots.size()) {
        if (indexToLook == -1) {
            int maxId = getMostRestricted();
            if (maxId == -1 ) {
                break;
            }
            _treeIndexes.push_back(maxId);
            indexToLook = _treeIndexes.size() - 1;
        } else {
            int maxId = getMostRestricted(_slots[_treeIndexes[indexToLook]]->getRestrictions());
            if (maxId == -1) {
                indexToLook--;
            } else {
                _treeIndexes.push_back(maxId);
                indexToLook = _treeIndexes.size() - 1;
            }
        }
    }

    for (int index : _treeIndexes) {
        std::cout << index << ", ";
    }
    std::cout << std::endl;
}

int Solver::getMostRestricted() {
    int maxId = -1;
    int maxRestrictions = -1;

    for (Slot* slot : _slots) {
        if (std::find(_treeIndexes.begin(), _treeIndexes.end(), slot->getId()) == _treeIndexes.end()) {
            if ((int)slot->getRestrictions().size() > maxRestrictions) {
                maxRestrictions = slot->getRestrictions().size();
                maxId = slot->getId();
            }
        }
    }

    return maxId;
}

int Solver::getMostRestricted(std::vector<Restriction*> restrictions) {
    int maxId = -1;
    int maxRestrictions = -1;

    for (Restriction* restriction : restrictions) {
        Slot* slot = _slots[restriction->getIdOtherWord()];
        if (std::find(_treeIndexes.begin(), _treeIndexes.end(), slot->getId()) == _treeIndexes.end()) {
            if ((int)slot->getRestrictions().size() > maxRestrictions) {
                maxRestrictions = slot->getRestrictions().size();
                maxId = slot->getId();
            }
        }
    }

    return maxId;
}

std::vector<Slot*> Solver::getSlots() const {
    return _slots;
}

bool Solver::solve(const Dictionary* dictionary) {
    int index = 0;
    while(index < _slots.size()) {
        std::vector<std::string> words = dictionary->getWordsByLength(_slots[_treeIndexes[index]]->getSize());
        int wordIndex = _slots[_treeIndexes[index]]->getWordId();
        while(wordIndex < words.size()) {
            std::vector<Restriction*> restrictions = _slots[_treeIndexes[index]]->getRestrictions();
            if (std::all_of(restrictions.begin(), restrictions.end(), [&](Restriction* restriction) {
                std::string otherWord = _slots[restriction->getIdOtherWord()]->getWord();
                return otherWord == "" || words[wordIndex][restriction->getMyLetterPos()] == otherWord[restriction->getOtherWordLetterPos()];
            })) {
                _slots[_treeIndexes[index]]->setWord(words[wordIndex]);
                _slots[_treeIndexes[index]]->setWordId(wordIndex);
                break;
            } else {
                wordIndex++;
            }
        }
        if (wordIndex >= words.size()) {
            _slots[_treeIndexes[index]]->setWord("");
            _slots[_treeIndexes[index]]->setWordId(0);
            index--;
            if (index == -1 ) {
                break;
            }
            _slots[_treeIndexes[index]]->setWord("");
            _slots[_treeIndexes[index]]->setWordId(_slots[_treeIndexes[index]]->getWordId() + 1);
            
        } else {
            index++;
        }
    }

    return index != -1;
}


