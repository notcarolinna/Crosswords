#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

class CrossWord {
private:
    std::string filename;
    std::vector<std::vector<char>> grid;
    std::unordered_map<int, std::vector<std::string>> words;

public:
    CrossWord();
    CrossWord(std::string filename);

    void WordStream();
    std::vector<std::vector<char>> getGrid();
    std::unordered_map<int, std::vector<std::string>> getWords();
};

CrossWord::CrossWord(std::string filename) : filename(filename) {
    WordStream();
}

void CrossWord::WordStream() {
    std::ifstream file("./resources/" + filename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Falha ao abrir o arquivo " << filename << std::endl;
        return;
    }

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
    CrossWord CrossWord("lista_palavras.txt");

    return 0;
}