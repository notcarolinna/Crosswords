#ifndef SLOT_H
#define SLOT_H

#include <utility>
#include <vector>
#include <iostream>


#include "restriction.h"

class Slot {

public:
    enum Direction {
        HORIZONTAL,
        VERTICAL
    };

    int getId() const;
    void setId(const int id);

    int getSize() const;
    void setSize(const int size);

    std::pair<int, int> getStartPos() const;
    void setStartPos(const std::pair<int, int>& startPos);

    Direction getDirection() const;
    void setDirection(const Direction& direction);

    std::vector<Restriction*> getRestrictions() const;
    void setRestrictions(const std::vector<Restriction*>& restrictions);
    void addRestriction(Restriction* restrictions);

    std::string getWord() const;
    void setWord(const std::string word);

    int getWordId() const;
    void setWordId(const int id);

    friend std::ostream& operator<<(std::ostream& os, Slot* s);

private:
    int _id;
    int _size;
    std::pair<int, int> _startPos;
    Direction _direction;
    std::vector<Restriction*> _restrictions;
    std::string _word;
    int _wordId;

};

#endif // SLOT_H