#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

class CrossWord {
private:
    std::string gridFilename;
    std::string wordsFilename;
    std::vector<std::vector<char>> grid;
    std::unordered_map<int, std::vector<std::string>> words;

public:
    CrossWord(std::string gridFile, std::string wordFile) : gridFilename(gridFile), wordsFilename(wordFile) {}

    bool GridStream();
    void WordStream();
    std::vector<std::vector<char>> getGrid();
    std::unordered_map<int, std::vector<std::string>> getWords();
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

        for (char c : line) {
            row.push_back(c);
        }

        grid.push_back(row);
    }

    file.close();

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

    for (const auto& pair : words) {
        int length = pair.first;
        const std::vector<std::string>& wordList = pair.second;

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

    std::cout<<"\n\nGrid: \n";
    crossword.GridStream();
    crossword.WordStream();

    return 0;
}