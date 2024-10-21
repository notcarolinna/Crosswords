#include "slot.h"

int Slot::getId() const {
    return _id;
}

void Slot::setId(const int id) {
    _id = id;
}

int Slot::getSize() const {
    return _size;
}

void Slot::setSize(const int size) {
    _size = size;
}

std::pair<int, int> Slot::getStartPos() const {
    return _startPos;
}

void Slot::setStartPos(const std::pair<int, int>& startPos) {
    _startPos = startPos;
}

Slot::Direction Slot::getDirection() const {
    return _direction;
}

void Slot::setDirection(const Slot::Direction& direction) {
    _direction = direction;
}

std::vector<Restriction*> Slot::getRestrictions() const {
    return _restrictions;
}

void Slot::setRestrictions(const std::vector<Restriction*>& restrictions) {
    _restrictions = restrictions;
}

void Slot::addRestriction(Restriction* restriction) {
    _restrictions.push_back(restriction);
}

std::string Slot::getWord() const {
    return _word;
}

void Slot::setWord(const std::string word) {
    _word = word;
}

int Slot::getWordId() const {
    return _wordId;
}

void Slot::setWordId(const int id) {
    _wordId = id;
}

std::ostream& operator<<(std::ostream& os, Slot* s) {
    os << "[ID] " << s->getId() << " [SIZE] " << s->getSize() << " [STARTPOS] (" << s->getStartPos().first << ", "
        << s->getStartPos().second << ") [DIRECTION] " << s->getDirection() << " [WORD] " << s->getWord() << " [WORD_ID] "
        << s->getWordId() << " [RESTRICTIONS] " << s->getRestrictions().size() << ": " << s->getRestrictions();
    return os;
}
