#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

class Attribute {
private:
    bool hasLetters;
    bool hasSpaces;

public:
    bool isEmpty() const { return hasSpaces && !hasLetters; }
    bool isPartial() const { return hasSpaces && hasLetters; }
    bool isFull() const { return !hasSpaces && hasLetters; }

    bool getHasLetters() const { return hasLetters; }
    void setHasLetters(bool value) { hasLetters = value; }

    bool getHasSpaces() const { return hasSpaces; }
    void setHasSpaces(bool value) { hasSpaces = value; }
};

#endif // ATTRIBUTE_H