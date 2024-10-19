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

std::vector<int> Solver::getTreeIndexes() const {
    return _treeIndexes;
}

