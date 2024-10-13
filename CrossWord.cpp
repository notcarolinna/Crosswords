#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm> // Para std::sort

class CrossWord {
private:
    std::string gridFilename;
    std::string wordsFilename;
    std::vector<std::vector<char>> grid;
    std::unordered_map<int, std::vector<std::string>> words;
    int numRows; 
    int numCols;   

public:
    CrossWord(std::string gridFile, std::string wordFile) 
        : gridFilename(gridFile), wordsFilename(wordFile), numRows(0), numCols(0) {}

    bool GridStream();
    void WordStream();
    std::vector<std::vector<char>> getGrid();
    std::unordered_map<int, std::vector<std::string>> getWords();
    int getNumRows() const { return numRows; } 
    int getNumCols() const { return numCols; }  
};

bool CrossWord::GridStream() {
    std::ifstream file("./resources/" + gridFilename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Falha ao abrir o arquivo " << gridFilename << std::endl;
        return false;
    }

    while (std::getline(file, line)) {
        std::vector<char> row;
        
        if (line.length() > 0) {
            numCols = line.length();
            numRows++;  
        }

        for (char c : line) {
            row.push_back(c);
        }

        grid.push_back(row);
    }

    file.close();

    std::cout << numRows << " x " << numCols << "\n" << std::endl;

    for (const std::vector<char>& row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

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
        int wordLength = word.length();
        words[wordLength].push_back(word);
    }

    file.close();

    std::vector<int> lengths; // Armazena os comprimentos das palavras
    for (const auto& pair : words) {
        lengths.push_back(pair.first);
    }

    std::sort(lengths.begin(), lengths.end());

    for (int length : lengths) {
        const std::vector<std::string>& wordList = words[length];

        std::cout << "\nPalavras com " << length << " letras:\n";
        for (const std::string& word : wordList) {
            std::cout << word << "\n";
        }
    }
}

std::vector<std::vector<char>> CrossWord::getGrid() {
    return grid;
}

std::unordered_map<int, std::vector<std::string>> CrossWord::getWords() {
    return words;
}

int main() {
    CrossWord crossword("grid-11x11.txt", "teste.txt");  

    std::cout << "\n\nGrid:";
    crossword.GridStream();
    crossword.WordStream();

    return 0;
}
