#ifndef GRID_H
#define GRID_H
#include <vector>
#include "interval.h"

class Grid {
private:
    Intervals intervals;

public:
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
    void loadFromFile(std::string fileName);
    void verify() const;
    void print() const;
    void printIntervals() const;

    const Intervals& getIntervals() const { return intervals; }
    std::string getString(const Interval& s, Attribute& attribute) const;

    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    //const std::vector<std::string>& getRows() const { return rows_; }
    void setRows(const std::vector<std::string>& r) { rows_ = r; }

    const Intervals& getIntervals() const { return intervals; }

private:
    std::string name;
    std::vector<std::string> rows_;
};

#endif // GRID_H