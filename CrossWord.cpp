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
    bool Backtrack(size_t wordIndex, const std::vector<std::string>& wordList);
    void RemoveWord(const std::string& word, size_t row, size_t col, int direction);
    void PrintGrid() const; 
    void PrintWords() const;

    std::vector<std::vector<char>> getGrid() const;
    std::unordered_map<size_t, std::vector<std::string>> getWords() const;

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
        //se a palavra não cabe na linha ou se a posição anterior ou posterior não for um '?'
        if (col + wordLength > numCols || (col > 0 && grid[row][col - 1] != '?') || (col + wordLength < numCols && grid[row][col + wordLength] != '?')) {
            std::cout << "Palavra " << word << " não cabe na linha " << row << std::endl;
            return false; 
        }
        for (size_t i = 0; i < wordLength; i++) { 
            char currentChar = grid[row][col + i];
            // se a posição contiver um '.' ou se a posição não estiver disponível e o caractere atual 
            // for diferente do caractere da palavra e diferente de '?'
            if (currentChar == '.' || (!availablePositions[row][col + i] && currentChar != word[i] && currentChar != '?')) {
                std::cout << "Palavra " << word << " não pode ser colocada na linha " << row << std::endl;
                return false;  
            }
        }
    } else {
        if (row + wordLength > numRows || (row > 0 && grid[row - 1][col] != '?') || (row + wordLength < numRows && grid[row + wordLength][col] != '?')) {
            std::cout << "Palavra " << word << " não cabe na coluna " << col << std::endl;
            return false; 
        }
        for (size_t i = 0; i < wordLength; i++) {
            char currentChar = grid[row + i][col];
            if (currentChar == '.' || (!availablePositions[row + i][col] && currentChar != word[i] && currentChar != '?')) {
                std::cout << "Palavra " << word << " não pode ser colocada na coluna " << col << std::endl;
                return false;  
            }
        }
    }

    std::cout << "Palavra " << word << " pode ser colocada." << std::endl;
    return true; 
}

void CrossWord::PlaceWord(const std::string& word, size_t row, size_t col, int direction) {
    if (Placeable(word, row, col, direction)) {
        size_t wordLength = word.length();
        std::cout << "Colocando a palavra " << word << " na linha " << row << " e coluna " << col << std::endl;

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
    }
}

void CrossWord::RemoveWord(const std::string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();
    std::cout << "Removendo a palavra " << word << " da linha " << row << " e coluna " << col << std::endl;

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

    for (size_t row = 0; row < numRows; row++) {
        for (size_t col = 0; col < numCols; col++) {
            for (int direction = 0; direction < 2; direction++) { // 0 = horizontal, 1 = vertical
                if (Placeable(word, row, col, direction)) {
                    PlaceWord(word, row, col, direction); // Coloca a palavra
                    if (Backtrack(wordIndex + 1, wordList)) { // Tenta a próxima palavra
                        return true; // Se todas as palavras foram colocadas com sucesso
                    }
                    RemoveWord(word, row, col, direction); // Remove se não foi possível colocar
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

void CrossWord::PrintWords() const {
    std::cout << "\n\nPalavras lidas:" << std::endl;

    std::vector<int> keys;
    for (const auto& pair : words) {
        keys.push_back(pair.first);
    }

    std::sort(keys.begin(), keys.end(), std::greater<int>());

    for(int key : keys) {
        std::cout << key << " :";
        for (const std::string& word : words.at(key)) {
            std::cout << " " << word;
        }
        std::cout << std::endl;
    }
}
std::vector<std::vector<char>> CrossWord::getGrid() const {
    return grid;
}

std::unordered_map<size_t, std::vector<std::string>> CrossWord::getWords() const {
    return words;
}

int main() {
    CrossWord crossword("grid-7x7.txt", "teste.txt");

    if (!crossword.GridStream()) {
        std::cerr << "Falha ao carregar a grade." << std::endl;
        return 1;
    }

    crossword.WordStream();

    std::vector<std::string> wordList;
    for (const auto& entry : crossword.getWords()) {
        wordList.insert(wordList.end(), entry.second.begin(), entry.second.end());
    }

    if (crossword.Backtrack(0, wordList)) {
        std::cout << "Solução encontrada." << std::endl;
    } else {
        std::cout << "Nenhuma solução encontrada." << std::endl;
    }

    crossword.PrintGrid(); 

    return 0;
}
