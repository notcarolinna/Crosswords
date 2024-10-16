#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>
#include <iostream>
#include <fstream>

class TrieNode {
public:
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool is_end_of_word;

    TrieNode() : is_end_of_word(false) {}
};

class Trie {
public:
    Trie() : root(std::make_shared<TrieNode>()) {}

    void insert(const std::string& word) {
        auto node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = std::make_shared<TrieNode>();
            }
            node = node->children[ch];
        }
        node->is_end_of_word = true;
    }

    std::vector<std::string> search_with_pattern(const std::string& pattern) {
        return search_with_pattern_recursive(root, pattern, 0, "");
    }

private:
    std::shared_ptr<TrieNode> root;

    std::vector<std::string> search_with_pattern_recursive(std::shared_ptr<TrieNode> node, const std::string& pattern, int index, const std::string& current_word) {
        if (index == pattern.size()) {
            if (node->is_end_of_word) {
                return {current_word};
            }
            return {};
        }

        char ch = pattern[index];
        std::vector<std::string> words;

        if (ch == '?') {
            for (const auto& [child_char, child_node] : node->children) {
                auto child_words = search_with_pattern_recursive(child_node, pattern, index + 1, current_word + child_char);
                words.insert(words.end(), child_words.begin(), child_words.end());
            }
        } else if (node->children.find(ch) != node->children.end()) {
            auto child_words = search_with_pattern_recursive(node->children[ch], pattern, index + 1, current_word + ch);
            words.insert(words.end(), child_words.begin(), child_words.end());
        }

        return words;
    }
};

// Funções relacionadas ao processamento de palavras
std::unordered_map<int, std::vector<std::string>> preprocess_words_by_length(const std::vector<std::string>& words) {
    std::unordered_map<int, std::vector<std::string>> word_size_map;
    for (const auto& word : words) {
        word_size_map[word.size()].push_back(word);
    }
    return word_size_map;
}

int count_vowels(const std::string& word) {
    const std::set<char> vowels = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    return std::count_if(word.begin(), word.end(), [&](char ch) { return vowels.find(ch) != vowels.end(); });
}

int priority_removal_criteria(const std::string& word) {
    int score = 0;
    const std::set<char> special_chars = {'&', '#', '$', '+', '!', '@', '*', '(', ')', '-', '_', '/'};
    const std::set<char> numbers = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    for (char ch : word) {
        if (special_chars.find(ch) != special_chars.end()) {
            score -= 30;
        } else if (numbers.find(ch) != numbers.end()) {
            score -= 20;
        } else {
            score += 1;
        }
    }
    return score;
}

std::vector<std::string> prioritize_words(const std::vector<std::string>& matching_words) {
    std::vector<std::string> sorted_words = matching_words;
    std::sort(sorted_words.begin(), sorted_words.end(), [](const std::string& word1, const std::string& word2) {
        return std::make_tuple(count_vowels(word1), priority_removal_criteria(word1)) >
               std::make_tuple(count_vowels(word2), priority_removal_criteria(word2));
    });
    return sorted_words;
}

// Funções para manipulação do grid
std::vector<std::vector<char>> load_grid(const std::string& file_path) {
    std::vector<std::vector<char>> grid;
    std::ifstream file(file_path);
    std::string line;

    while (std::getline(file, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }

    return grid;
}

std::string last_printed_grid;

void print_grid(const std::vector<std::vector<char>>& grid) {
    std::string current_grid;
    for (const auto& row : grid) {
        current_grid += std::string(row.begin(), row.end()) + '\n';
    }

    if (current_grid != last_printed_grid) {
        last_printed_grid = current_grid;
        std::cout << current_grid;
    }
}

bool is_grid_complete(const std::vector<std::vector<char>>& grid) {
    for (const auto& row : grid) {
        if (std::find(row.begin(), row.end(), '?') != row.end()) {
            return false;
        }
    }
    return true;
}

std::vector<std::tuple<std::string, int, int, std::string>> get_all_words_from_grid(const std::vector<std::vector<char>>& grid) {
    std::vector<std::tuple<std::string, int, int, std::string>> words_found;

    // Captura palavras horizontais
    for (int row = 0; row < grid.size(); row++) {
        int col = 0;
        while (col < grid[0].size()) {
            if (grid[row][col] != '.') {
                int start_col = col;
                while (col < grid[0].size() && grid[row][col] != '.') {
                    col++;
                }
                std::string word(grid[row].begin() + start_col, grid[row].begin() + col);
                if (word.size() > 1) {
                    words_found.emplace_back(word, row, start_col, "H");
                }
            } else {
                col++;
            }
        }
    }

    // Captura palavras verticais
    for (int col = 0; col < grid[0].size(); col++) {
        int row = 0;
        while (row < grid.size()) {
            if (grid[row][col] != '.') {
                int start_row = row;
                while (row < grid.size() && grid[row][col] != '.') {
                    row++;
                }
                std::string word;
                for (int i = start_row; i < row; i++) {
                    word += grid[i][col];
                }
                if (word.size() > 1) {
                    words_found.emplace_back(word, start_row, col, "V");
                }
            } else {
                row++;
            }
        }
    }

    return words_found;
}

bool validate_all_words_in_grid(const std::vector<std::vector<char>>& grid, Trie& words_trie) {
    auto all_words = get_all_words_from_grid(grid);
    std::vector<std::tuple<std::string, int, int, std::string>> invalid_words;

    for (const auto& [word, row, col, direction] : all_words) {
        if (words_trie.search_with_pattern(word).empty()) {
            invalid_words.emplace_back(word, row, col, direction);
        }
    }

    if (!invalid_words.empty()) {
        for (const auto& [word, row, col, direction] : invalid_words) {
            std::cout << "Palavra inválida encontrada: " << word << " na direção " << direction 
                      << " na posição (" << row << ", " << col << ")\n";
        }
        return false;
    }
    return true;
}

// Função principal de interação com o grid
void place_word(std::vector<std::vector<char>>& grid, const std::string& word, int row, int col, const std::string& direction) {
    std::cout << "Colocando palavra '" << word << "' na posição (" << row << ", " << col << ") na direção " << direction << ".\n";

    if (direction == "H") {
        for (int i = 0; i < word.length(); i++) {
            grid[row][col + i] = word[i];
        }
    } else if (direction == "V") {
        for (int i = 0; i < word.length(); i++) {
            grid[row + i][col] = word[i];
        }
    }

    print_grid(grid);
}

void remove_word(std::vector<std::vector<char>>& grid, const std::string& word, int row, int col, const std::string& direction) {
    std::cout << "Removendo palavra '" << word << "' da posição (" << row << ", " << col << ") na direção " << direction << ".\n";
    int length = word.length();

    if (direction == "H") {
        for (int i = 0; i < length; i++) {
            if (col + i < grid[0].size() && grid[row][col + i] == word[i]) {
                grid[row][col + i] = '?';
            }
        }
    } else if (direction == "V") {
        for (int i = 0; i < length; i++) {
            if (row + i < grid.size() && grid[row + i][col] == word[i]) {
                grid[row + i][col] = '?';
            }
        }
    }

    print_grid(grid);
}

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Função para verificar se a palavra está no arquivo
bool verify_word(const std::string& file_path, const std::string& searched_word) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Arquivo não encontrado." << std::endl;
        return false;
    }

    std::string line;
    std::string upper_searched_word = to_upper(searched_word); // Converte a palavra buscada para maiúsculas

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (to_upper(word) == upper_searched_word) { // Compara as palavras ignorando maiúsculas/minúsculas
                std::cout << "A palavra '" << searched_word << "' está na lista." << std::endl;
                return true;
            }
        }
    }

    std::cout << "A palavra '" << searched_word << "' não está na lista." << std::endl;
    return false;
}


// Função para verificar se o grid está completo
bool is_grid_complete(const std::vector<std::vector<char>>& grid) {
    // Adicione a lógica aqui para verificar se o grid está completo
    return true;  // Exemplo fictício
}

// Função para validar todas as palavras no grid usando a Trie
bool validate_all_words_in_grid(const std::vector<std::vector<char>>& grid, Trie& words_trie) {
    // Adicione a lógica de validação aqui
    return true;  // Exemplo fictício
}

// Função para remover palavras que cruzam palavras inválidas
void remove_intersecting_words_for_invalid(std::vector<std::vector<char>>& grid, const std::string& word, 
                                           int row, int col, const std::string& direction, 
                                           std::vector<std::tuple<std::string, int, int, std::string>>& used_words, 
                                           Trie& words_trie) {
    // Adicione a lógica de remoção aqui
}

// Função para tentar encontrar uma nova palavra que substitua uma palavra inválida
void place_word(std::vector<std::vector<char>>& grid, const std::string& word, 
                int row, int col, const std::string& direction) {
    // Coloque a palavra no grid na posição correta
}

// Função principal de resolução usando backtracking
bool solve(std::vector<std::vector<char>>& grid, Trie& words_trie, 
           std::map<int, std::vector<std::string>>& word_size_map, 
           std::vector<std::tuple<std::string, int, int, std::string>>& used_words, 
           std::map<std::tuple<int, int, std::string>, int>& attempt_count, 
           int depth = 0, int max_depth = 99999, 
           std::map<std::tuple<int, int, std::string>, std::set<std::string>>& tried_words, 
           std::vector<std::string>& removed_words, int retry_limit = 10, 
           std::map<std::string, int>& removal_attempts) {

    // Verifica se o grid está completo
    if (is_grid_complete(grid)) {
        // Valida todas as palavras
        if (validate_all_words_in_grid(grid, words_trie)) {
            std::cout << "O jogo foi concluído com sucesso!" << std::endl;
            // Imprimir as palavras usadas e suas posições
            for (const auto& [word, row, col, direction] : used_words) {
                std::cout << "Palavra: " << word << " | Posição: (" << row << ", " << col << ") | Direção: " << direction << std::endl;
            }
            return true;
        } else {
            // Remover palavras inválidas
            for (const auto& [word, row, col, direction] : used_words) {
                if (!words_trie.search_with_pattern(word)) {
                    std::cout << "Palavra inválida detectada: " << word << std::endl;
                    remove_intersecting_words_for_invalid(grid, word, row, col, direction, used_words, words_trie);
                }
            }
        }
    }

    // Lógica adicional para tentar preencher o grid...

    return false;
}

int main() {
    // Exemplo de inicialização do grid e outros parâmetros
    std::vector<std::vector<char>> grid = { /* Inicialização do grid */ };
    Trie words_trie;  // Supondo que uma implementação de Trie esteja disponível
    std::map<int, std::vector<std::string>> word_size_map;  // Mapeamento de tamanhos de palavras
    std::vector<std::tuple<std::string, int, int, std::string>> used_words;
    std::map<std::tuple<int, int, std::string>, int> attempt_count;
    std::map<std::tuple<int, int, std::string>, std::set<std::string>> tried_words;
    std::vector<std::string> removed_words;
    std::map<std::string, int> removal_attempts;

    // Chama a função de resolução
    solve(grid, words_trie, word_size_map, used_words, attempt_count);
    
    return 0;
}
