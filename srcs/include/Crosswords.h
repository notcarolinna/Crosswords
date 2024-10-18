#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <chrono>
#include "Dictionary.h"
#include "Solver.h"
#include "Grid.h"


void loadAndVerifyGrid(Grid& grid, const std::string& filename);
void loadDictionary(Dictionary& dictionary, const std::string& filename, int maxSize);
void solveGrid(Solver& solver, Grid& grid);
void printCompletionTime(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);

int main();
