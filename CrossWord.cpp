#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <chrono>
using  namespace std;

class CrossWord {
private:
    string gridFilename;
    string wordsFilename;
    vector<vector<char>> grid;
    unordered_map<size_t, vector<string>> words;
    vector<vector<bool>> availablePositions;
    size_t numRows;
    size_t numCols;

public:
    CrossWord(string gridFile, string wordFile)
        : gridFilename(move(gridFile)),
          wordsFilename(move(wordFile)),
          numRows(0),
          numCols(0) {}

    void WordStream();
    bool GridStream();
    void CreateAvailablePositions();

    bool Placeable(const string& word, size_t row, size_t col, int direction);
    void PlaceWord(const string& word, size_t row, size_t col, int direction);
    void RemoveWord(const string& word, size_t row, size_t col, int direction);
    bool Backtrack(size_t wordIndex, const vector<string>& wordList);

    void PrintGrid() const;
    void PrintWords() const;

    vector<vector<char>> getGrid() const;
    unordered_map<size_t, vector<string>> getWords() const;
    vector<string> sorting() const;

    size_t getNumRows() const { return numRows; }
    size_t getNumCols() const { return numCols; }
};

void CrossWord::WordStream() {
    ifstream file("./resources/" + wordsFilename);
    string word;

    if (!file.is_open()) {
        cerr << "Failed to open the file " << wordsFilename << endl;
        return;
    }

    words.clear();

    while (file >> word) {
        if (all_of(word.begin(), word.end(), [](char c) {
            return isalpha(c);
        })) {
            words[word.length()].push_back(word);
        } else {
            cout << "Ignored word: " << word << endl;
        }
    }

    file.close();
    PrintWords();
}

bool CrossWord::GridStream() {
    ifstream file("./resources/" + gridFilename);
    string line;

    if (!file.is_open()) {
        cerr << "Failed to open the file " << gridFilename << endl;
        return false;
    }

    while (getline(file, line)) {
        vector<char> row(line.begin(), line.end());

        if (!row.empty()) {
            if (numCols == 0) {
                numCols = row.size();
            }
            numRows++;
            grid.push_back(row);
        }
    }

    file.close();

    cout << "\n" << numRows << " x " << numCols;
    CreateAvailablePositions();
    PrintGrid();
    return true;
}

void CrossWord::CreateAvailablePositions() {
    availablePositions = vector<vector<bool>>(numRows, vector<bool>(numCols, false));
    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {
            if (grid[i][j] == '?') {
                availablePositions[i][j] = true;
            }
            else {
                availablePositions[i][j] = false;
            }
        }
    }
}


bool CrossWord::Placeable(const string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();

    if (direction == 0) {
        if (col + wordLength > numCols) {
            // cout << "1 word " << word << " doesn't fit on line " << row << endl;
            return false;
        }
        if(col > 0 && grid[row][col - 1] != '.'){
            // cout << "2 word " << word << " doesn't fit on line " << row << endl;
            return false;
        }
        if (col + wordLength < numCols && grid[row][col + wordLength] != '.') {
            // cout << "3 word " << word << " doesn't fit on line " << row << endl;
            return false;
        }

        for (size_t i = 0; i < wordLength; i++) {
            char currentChar = grid[row][col + i];
            if (currentChar == '.' || (currentChar != word[i] && currentChar != '?')) {
                //cout << "word " << word << " can't be placed on line " << row << endl;
                return false;
            }
        }
        if(col + wordLength > numCols && grid[row][col + wordLength] != '.'){
            //cout << "word " << word << " não pode ser colocada na linha " << row << endl;
            return false;
        }
    }
    else {
        if (row + wordLength > numRows) {
            //cout << "\n1 word " << word << " não cabe na coluna " << col << endl;
            return false;
        }
        if(row > 0 && grid[row - 1][col] != '.'){
            //cout << "2 word " << word << " não cabe na coluna " << col << endl;
            return false;
        }
        if (row + wordLength < numRows && grid[row + wordLength][col] != '.') {
            //cout << "3 word " << word << " não cabe na coluna " << col << endl;
            return false;
        }

        for (size_t i = 0; i < wordLength; i++) {
            char currentChar = grid[row + i][col];
            if (currentChar == '.' || (currentChar != word[i] && currentChar != '?')) {
                //cout << "Palavra " << word << " não pode ser colocada na coluna " << col << endl;
                return false;
            }
        }
        if(row + wordLength > numRows && grid[row + wordLength][col] != '.'){
            //cout << "Palavra " << word << " não pode ser colocada na coluna " << row << endl;
            return false;
        }
    }

    cout << "\n\t  ***Word " << word << " can be placed.***" << endl;
    return true;
}

void CrossWord::PlaceWord(const string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();
    cout << "  ***Placing word " << word << " on line " << row << " and column " << col << "***\n" << endl;

    if (direction == 0) {
        for (size_t i = 0; i < wordLength; i++) {
            grid[row][col + i] = word[i];
            //availablePositions[row][col + i] = false;
        }
    } else {
        for (size_t i = 0; i < wordLength; i++) {
            grid[row + i][col] = word[i];
            //availablePositions[row + i][col] = false;
        }
    }
}

void CrossWord::RemoveWord(const string& word, size_t row, size_t col, int direction) {
    size_t wordLength = word.length();
    cout << "\n  ***Removing the word " << word << " from line " << row << " and column " << col << "***\n" << endl;

    if (direction == 0) {
        for (size_t i = 0; i < wordLength; i++) {
            if (grid[row][col + i] == word[i]) {
                grid[row][col + i] = '?';
                //availablePositions[row][col + i] = true;
            }
        }
    } else {
        for (size_t i = 0; i < wordLength; i++) {
            if (grid[row + i][col] == word[i]) {
                grid[row + i][col] = '?';
                //availablePositions[row + i][col] = true;
            }
        }
    }
}

bool CrossWord::Backtrack(size_t wordIndex, const vector<string>& wordList) {

    bool completo = true;
    CreateAvailablePositions();

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            cout << availablePositions[i][j] << " ";
            if(availablePositions[i][j]){
                completo = false;
            }
        }
        cout << endl;
    }

    cout << "\n" << endl;

    if(completo){
        cout << "All positions are filled." << endl;
        return true;
    }
    else if (wordIndex == wordList.size()) {
        cout << "No more words to work with." << endl;
        return false;
    }

    auto gridCopy = grid;

    if(wordList[wordIndex].length() > numRows || wordList[wordIndex].length() > numCols) {
        for(int i = wordIndex; i < wordList.size(); i++) {
            const string& aux = wordList[i];
            if(wordList[i].length() > numRows && aux.length() > numCols) {
                cout << "Word " << aux << " doesn't fit on grid." << endl;
            }
            else if (wordList[i].length() <= wordList[wordIndex].length()) {
                cout << "Word " << aux << " fits on grid." << endl;
                wordIndex = i;
                break;
            }
        }
    }

    const string& word = wordList[wordIndex];

    for(size_t row = 0; row < numRows; row++){
        for(size_t col = 0; col < numCols; col++){
            if(gridCopy[row][col] == '?' || gridCopy[row][col] == word[0]){

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
    cout << "\n\n";
    for (const auto& row : grid) {
        for (char c : row) {
            cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

void CrossWord::PrintWords() const {
    cout << "\n\n";

    vector<string> sortedWordsList = sorting();
    unordered_map<size_t, vector<string>> wordMap;

    for (const string& word : sortedWordsList) {
        wordMap[word.size()].push_back(word);
    }

    vector<size_t> keys;
    for (const auto& entry : wordMap) {
        keys.push_back(entry.first);
    }

    sort(keys.begin(), keys.end(), greater<size_t>());

    for (size_t key : keys) {
        cout << key << " : ";
        vector<string>& words = wordMap[key];
        sort(words.begin(), words.end(), greater<string>());

        for (const string& word : words) {
            cout << word << " ";
        }
        cout << endl;
    }
}


vector<vector<char>> CrossWord::getGrid() const {
    return grid;
}

unordered_map<size_t, vector<string>> CrossWord::getWords() const {
    return words;
}

vector<string> CrossWord::sorting() const {
    unordered_map<size_t, vector<string>> sortedWords;
    vector<string> sortedWordsList;

    vector<int> keys;
    for (const auto& pair : words) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end(), greater<int>());

    for(int key : keys) {
        sortedWords[key] = words.at(key);
        for (const string& word : sortedWords[key]) {
            sortedWordsList.push_back(word);
        }
    }
    return sortedWordsList;
}


int main() {
    CrossWord crossword("grid-teste.txt", "teste2.txt");

    if (!crossword.GridStream()) {
        cerr << "Failed to load grid." << endl;
        return 1;
    }

    crossword.WordStream();
    vector<string> wordList = crossword.sorting();

    cout << endl;

    if (crossword.Backtrack(0, wordList)) {
        cout << "Solution found." << endl;
    } else {
        cout << "No solution found." << endl;
    }

    crossword.PrintGrid();

    return 0;
}
