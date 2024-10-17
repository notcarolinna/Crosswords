#ifndef RESTRICTION_H
#define RESTRICTION_H

#include <iostream>
#include <vector>

class Restriction {

public:
    int getMyLetterPos() const;
    void setMyLetterPos(const int myLetterPos);

    int getIdOtherWord() const;
    void setIdOtherWord(const int idOtherWord);

    int getOtherWordLetterPos() const;
    void setOtherWordLetterPos(const int otherWordLetterPos);

    friend std::ostream& operator<<(std::ostream& os, Restriction* s);
    friend std::ostream& operator<<(std::ostream& os, std::vector<Restriction*> v);

private:
    int _myLetterPos;
    int _idOtherWord;
    int _otherWordLetterPos;

};

#endif // RESTRICTION_H