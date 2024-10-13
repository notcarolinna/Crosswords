#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm> 
#include <tuple>

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
    
    void PrintGrid() const; 

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

    std::cout << numRows << " x " << numCols << "\n" << std::endl;
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
        words[word.length()].push_back(word);
    }

    file.close();

    std::vector<int> lengths;
    for(const auto& pair : words) {
        lengths.push_back(pair.first);
    }

    std::sort(lengths.begin(), lengths.end());

    for(int length : lengths) {
        std::cout << length << " : ";
        for(const std::string& word : words[length]) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
}

bool CrossWord::Placeable(const std::string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();

    std::cout << "Verificando se a palavra '" << word << "' pode ser inserida em (" 
              << row << ", " << col << ") na direção " << direction << std::endl;
    PrintGrid();

    if (direction == 0) {  // Horizontal
        if (col + wordLength > numCols) {
            return false; 
        }
        for (size_t i = 0; i < wordLength; i++) { 
            std::cout << "\nVerificando posição (" << row << ", " << col + i << "): ";
            if (!availablePositions[row][col + i] && grid[row][col + i] != word[i]) { 
                std::cout << "\n '" << grid[row][col+i] << "' diferente de '" << word[i] << "'" << std::endl;
                return false;  
            }
        }
    } else {  // Vertical
        if (row + wordLength > numRows) {
            return false; 
        }
        for (size_t i = 0; i < wordLength; i++) {
            std::cout << "\nVerificando posição (" << row << ", " << col + i << "): ";
            if (!availablePositions[row + i][col] && grid[row + i][col] != word[i]) {
                std::cout << "\n" << grid[row][col+i] << "' diferente de '" << word[i] << "'" << std::endl;
                return false;  
            }
        }
    }

    return true; 
}

void CrossWord::PlaceWord(const std::string& word, size_t row, size_t col, int direction) {
    if (Placeable(word, row, col, direction)) {
        size_t wordLength = word.length();
        std::cout << "\n\nInserindo '" << word << "' em (" << row << ", " << col << ") na direção " << direction << std::endl;

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

        std::cout << "Grid atualizado após inserir '" << word << std::endl;
        PrintGrid(); 
    } else {
        std::cout << "Não é possível inserir a palavra '" << word << "' em (" << row << ", " << col << ") na direção " << direction << std::endl;
    }
}


void CrossWord::PrintGrid() const {
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

std::unordered_map<size_t, std::vector<std::string>> CrossWord::getWords() const {
    return words;
}

int main() {
    CrossWord crossword("grid-teste.txt", "teste2.txt");  

    if (!crossword.GridStream()) {
        std::cerr << "Falha ao carregar a grade." << std::endl;
        return 1;
    }

    crossword.WordStream();

    // Linha, Coluna e Direção (0 = Horizontal, 1 = Vertical)
    std::vector<std::tuple<std::string, size_t, size_t, int>> testWords = {
        {"COELHO", 0, 1, 1}, 
        {"BOTA", 1, 0, 0},   
        {"AZUL", 1, 3, 1},   
        {"CHULO", 4, 0, 0} 
    };

    for (const auto& wordInfo : testWords) {
        std::string word;
        size_t row, col;
        int direction;

        std::tie(word, row, col, direction) = wordInfo;

        if (crossword.Placeable(word, row, col, direction)) {
            std::cout << "\nA palavra '" << word << "' pode ser inserida em (" 
                      << row << ", " << col << ") na direção " << direction << std::endl;

            crossword.PlaceWord(word, row, col, direction);
        } else {
            std::cout << "\nA palavra '" << word << "' NÃO pode ser inserida em (" 
                      << row << ", " << col << ") na direção " << direction << std::endl;
        }
    }

    return 0;
}
