#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>

class Grid {
public:
    struct Point {
        Point() {}
        Point(int r, int c) : row(r), col(c) {}

        friend std::ostream& operator<<(std::ostream& os, const Point& p);
        int row = 0;
        int col = 0;
    };

    struct Interval {
        Interval(Point p, int l, bool v) : point(p), length(l), vertical(v) {}
        Point getPoint(int i) const;
        friend std::ostream& operator<<(std::ostream& os, const Interval& s);

        Point point;
        int length;
        bool vertical;
    };

    struct Attribute {
        bool isEmpty() const { return hasSpaces && !hasLetters; }
        bool isPartial() const { return hasSpaces && hasLetters; }
        bool isFull() const { return !hasSpaces && hasLetters; }
        bool hasLetters = false;
        bool hasSpaces = false;
    };

    typedef std::vector<Interval> Intervals;

    Grid();
    Grid(std::string n);

    int getRows() const;
    int getCols() const;
    int getMaxSize() const;
    char getCell(const Point& p) const;
    void setCell(const Point& p, char c);
    bool isBlock(const Point& p) const;
    bool isSpace(const Point& p) const;
    bool isLetter(const Point& p) const;
    bool isInside(const Point& p) const;
    std::string getString(const Interval& s, Attribute& attribute) const;
    void writeString(const Interval& s, const std::string& t);
    bool nextPoint(Point& p, bool vertical);
    bool nextPointStopOnWrap(Point& p, bool vertical);
    void fillIntervals(bool vertical);
    void fillIntervals();
    void loadFromFile(const std::string& fileName);
    void verify() const;
    void print() const;
    void printIntervals() const;

    const Intervals& getIntervals() const; // Método de acesso aos intervalos

private:
    std::string name;
    std::vector<std::string> rows_;
    Intervals intervals;
};