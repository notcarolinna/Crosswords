#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm> 
#include <tuple>
#include <chrono>

class CrossWord {
private:
    std::string gridFilename;
    std::string wordsFilename;
    std::vector<std::vector<char>> grid;
    std::unordered_map<size_t, std::vector<std::string>> words; 
    std::vector<std::vector<bool>> availablePositions;
    size_t numRows; 
    size_t numCols;   

public:
    CrossWord(std::string gridFile, std::string wordFile) 
        : gridFilename(std::move(gridFile)), 
          wordsFilename(std::move(wordFile)), 
          numRows(0), 
          numCols(0) {}

    void WordStream();
    bool GridStream();
    
    bool Placeable(const std::string& word, size_t row, size_t col, int direction);
    void PlaceWord(const std::string& word, size_t row, size_t col, int direction);
    void RemoveWord(const std::string& word, size_t row, size_t col, int direction);
    bool Backtrack(size_t wordIndex, const std::vector<std::string>& wordList);
    void PrintGrid() const; 
    void PrintWords() const;

    std::vector<std::vector<char>> getGrid() const;
    std::unordered_map<size_t, std::vector<std::string>> getWords() const;
    std::vector<std::string> sorting() const;

    size_t getNumRows() const { return numRows; } 
    size_t getNumCols() const { return numCols; }
};

bool CrossWord::GridStream() {
    std::ifstream file("./resources/" + gridFilename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Falha ao abrir o arquivo " << gridFilename << std::endl;
        return false;
    }

    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end()); 

        if (!row.empty()) {
            if (numCols == 0) {
                numCols = row.size();
            }
            numRows++;  
            grid.push_back(row);
        }
    }

    file.close();

    availablePositions = std::vector<std::vector<bool>>(numRows, std::vector<bool>(numCols, true));

    std::cout << "\n" << numRows << " x " << numCols << std::endl;
    PrintGrid(); 
    return true;
}

void CrossWord::WordStream() {
    std::ifstream file("./resources/" + wordsFilename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Falha ao abrir o arquivo " << wordsFilename << std::endl;
        return;
    }

    words.clear();

    while (file >> word) {
        if (std::all_of(word.begin(), word.end(), [](char c) {
            return std::isalpha(c);
        })) {
            words[word.length()].push_back(word);
        } else {
            std::cout << "Palavra ignorada: " << word << std::endl;
        }
    }

    file.close();
    PrintWords();
}

bool CrossWord::Placeable(const std::string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();

    if (direction == 0) {
        if (col + wordLength > numCols) {
            std::cout << "1 Palavra " << word << " não cabe na linha " << row << std::endl;
            return false; 
        }
        if(col > 0 && grid[row][col - 1] != '.'){
            std::cout << "2 Palavra " << word << " não cabe na linha " << row << std::endl;
            return false;
        }
        if (col + wordLength < numCols && grid[row][col + wordLength] != '.') {
            std::cout << "3 Palavra " << word << " não cabe na linha " << row << std::endl;
            return false; 
        }

        for (size_t i = 0; i < wordLength; i++) { 
            char currentChar = grid[row][col + i];
            if (currentChar == '.' || (currentChar != word[i] && currentChar != '?')) {
                std::cout << "Palavra " << word << " não pode ser colocada na linha " << row << std::endl;
                return false;  
            }
        }
        if(col + wordLength > numCols && grid[row][col + wordLength] != '.'){
            std::cout << "Palavra " << word << " não pode ser colocada na linha " << row << std::endl;
            return false;
        }
    } 
    else {
        if (row + wordLength > numRows) {
            std::cout << "\n1 Palavra " << word << " não cabe na coluna " << col << std::endl;
            return false; 
        }
        if(row > 0 && grid[row - 1][col] != '.'){
            std::cout << "2 Palavra " << word << " não cabe na coluna " << col << std::endl;
            return false;
        }
        if (row + wordLength < numRows && grid[row + wordLength][col] != '.') {
            std::cout << "3 Palavra " << word << " não cabe na coluna " << col << std::endl;
            return false; 
        }

        for (size_t i = 0; i < wordLength; i++) {
            char currentChar = grid[row + i][col];
            if (currentChar == '.' || (currentChar != word[i] && currentChar != '?')) {
                std::cout << "Palavra " << word << " não pode ser colocada na coluna " << col << std::endl;
                return false;  
            }
        }
        if(row + wordLength > numRows && grid[row + wordLength][col] != '.'){
            std::cout << "Palavra " << word << " não pode ser colocada na coluna " << row << std::endl;
            return false;
        }
    }

    std::cout << "\n\t  ***Palavra " << word << " pode ser colocada.***" << std::endl;
    return true; 
}

void CrossWord::PlaceWord(const std::string& word, size_t row, size_t col, int direction) {
   // if (Placeable(word, row, col, direction)) {
        size_t wordLength = word.length();
        std::cout << "  ***Colocando a palavra " << word << " na linha " << row << " e coluna " << col << "***\n" << std::endl;

        if (direction == 0) { 
            for (size_t i = 0; i < wordLength; i++) {
                grid[row][col + i] = word[i];
                availablePositions[row][col + i] = false;  
            }
        } else {
            for (size_t i = 0; i < wordLength; i++) {
                grid[row + i][col] = word[i];
                availablePositions[row + i][col] = false;  
            }
        }
    //}
}

void CrossWord::RemoveWord(const std::string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();
    std::cout << "\n  ***Removendo a palavra " << word << " da linha " << row << " e coluna " << col << "***\n" << std::endl;

    if (direction == 0) {  
        for (size_t i = 0; i < wordLength; i++) {
            if (grid[row][col + i] == word[i]) {
                grid[row][col + i] = '?';  
                availablePositions[row][col + i] = true;
            }
        }
    } else {  
        for (size_t i = 0; i < wordLength; i++) {
            if (grid[row + i][col] == word[i]) {
                grid[row + i][col] = '?';
                availablePositions[row + i][col] = true;
            }
        }
    }
}

bool CrossWord::Backtrack(size_t wordIndex, const std::vector<std::string>& wordList) {
    if (wordIndex == wordList.size()) {
        std::cout << "Todas as palavras foram colocadas." << std::endl;
        return true;
    }

    const std::string& word = wordList[wordIndex];
    auto gridCopy = grid;

    for(size_t row = 0; row < numRows; row++){
        for(size_t col = 0; col < numCols; col++){

            //std::cout << "\nTentando colocar a palavra " << word << " na linha " << row << " e coluna " << col << std::endl;

            if(gridCopy[row][col] == '?' || gridCopy[row][col] == word[0]){
                //std::cout<< "\nTentando colocar a palavra " << word << " na linha " << row << " e coluna " << col << std::endl;

                if(Placeable(word, row, col, 0)){
                    PlaceWord(word, row, col, 0);
                    if(Backtrack(wordIndex + 1, wordList)){
                        return true;
                    }
                    RemoveWord(word, row, col, 0);
                }

                gridCopy = grid;

                if(Placeable(word, row, col, 1)){
                    PlaceWord(word, row, col, 1);
                    if(Backtrack(wordIndex + 1, wordList)){
                        return true;
                    }
                    RemoveWord(word, row, col, 1);
                }
            }
        }
    }
    return false;
}

void CrossWord::PrintGrid() const {
    std::cout << "\n\n";
    for (const auto& row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
}

std::vector<std::vector<char>> CrossWord::getGrid() const {
    return grid;
}

std::vector<std::string> CrossWord::sorting() const {
    std::unordered_map<size_t, std::vector<std::string>> sortedWords;
    std::vector<std::string> sortedWordsList;

    std::vector<int> keys;
    for (const auto& pair : words) {
        keys.push_back(pair.first);
    }

    std::sort(keys.begin(), keys.end(), std::greater<int>());

    for(int key : keys) {
        sortedWords[key] = words.at(key);
        std::cout << key << " : ";
        for (const std::string& word : sortedWords[key]) {
            std::cout << word << " ";
            sortedWordsList.push_back(word);
        }
        std::cout << std::endl;
    }

    return sortedWordsList;
}

std::unordered_map<size_t, std::vector<std::string>> CrossWord::getWords() const {
    return words;
}

void CrossWord::PrintWords() const {
    std::cout << "\n\n";
    std::vector<std::string> sortedWordsList = sorting();
    for(const std::string& word : sortedWordsList) {
        std::cout << word << std::endl;
    }
}

int main() {
    CrossWord crossword("grid-teste.txt", "teste2.txt");

    if (!crossword.GridStream()) {
        std::cerr << "Falha ao carregar a grade." << std::endl;
        return 1;
    }

    crossword.WordStream();

    std::vector<std::string> wordList = crossword.sorting();
    
    std::cout << "\n\nMain:" << std::endl;
    for(const std::string& word : wordList) {
        std::cout << word << std::endl;
    }

    std::cout << std::endl;

    if (crossword.Backtrack(0, wordList)) {
        std::cout << "Solução encontrada." << std::endl;
    } else {
        std::cout << "Nenhuma solução encontrada." << std::endl;
    }

    crossword.PrintGrid(); 

    return 0;
}
