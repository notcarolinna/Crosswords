#include "attribute.h"

// Implementação dos métodos da classe Attribute

bool Attribute::isEmpty() const {
    return hasSpaces && !hasLetters;
}

bool Attribute::isPartial() const {
    return hasSpaces && hasLetters;
}

bool Attribute::isFull() const {
    return !hasSpaces && hasLetters;
}

bool Attribute::getHasLetters() const {
    return hasLetters;
}

void Attribute::setHasLetters(bool value) {
    hasLetters = value;
}

bool Attribute::getHasSpaces() const {
    return hasSpaces;
}

void Attribute::setHasSpaces(bool value) {
    hasSpaces = value;
}