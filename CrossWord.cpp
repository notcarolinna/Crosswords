#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

class Reader {
private:
    std::string filename;
    std::vector<std::vector<char>> grid;
    std::unordered_map<int, std::vector<std::string>> words;

public:
    Reader();
    Reader(std::string filename);

    void WordStream();
    std::vector<std::vector<char>> getGrid();
    std::unordered_map<int, std::vector<std::string>> getWords();
};

Reader::Reader(std::string filename) : filename(filename) {
    WordStream();
}

void Reader::WordStream() {
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

std::vector<std::vector<char>> Reader::getGrid() {
    return grid;
}

std::unordered_map<int, std::vector<std::string>> Reader::getWords() {
    return words;
}

int main() {
    Reader reader("lista_palavras.txt");

    return 0;
}