#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iomanip>
#include <chrono>

#define MAX_PORTOS 9

class Dados {
private:
	int linhas;
	int colunas;
	std::string arquivo;

	std::vector<std::vector<char>> grafo; // Armazena o grafo
	std::unordered_map<int, std::pair<int, int>> mapa_coordenadas; // Mapa de coordenadas

public:
	Dados(std::string arquivo) : arquivo(arquivo) {}
	bool Mapa();
	void A_STAR();
	int Manhattan(const std::pair<int, int>& origem, const std::pair<int, int>& destino);
	int CalculaDistancia(const std::pair<int, int>& origem, const std::pair<int, int>& destino);
};

bool Dados::Mapa() {
	std::ifstream file("./resources/" + arquivo);

	if (!file.is_open()) {
		std::cerr << "Falha ao abrir o arquivo " << arquivo << std::endl;
		return false;
	}

	std::string line;
	std::getline(file, line);
	std::stringstream ss(line);
	ss >> linhas >> colunas;

	grafo = std::vector<std::vector<char>>(linhas, std::vector<char>(colunas));

	for (int i = 0; i < linhas; i++) {
		std::getline(file, line);
		std::vector<char> col(colunas, ' ');

		for (int j = 0; j < colunas && j < line.size(); j++) {
			col[j] = line[j];

			if (line[j] >= '1' && line[j] <= '9') { // Se o caractere for um número de 1 até 9
				int num = line[j] - '0';
				mapa_coordenadas.emplace(num, std::make_pair(i, j));
			}
		}
		grafo[i] = col;
	}
	return true;
}

int Dados::Manhattan(const std::pair<int, int>& origem, const std::pair<int, int>& destino) {
	return abs(origem.first - destino.first) + abs(origem.second - destino.second);
}

int Dados::CalculaDistancia(const std::pair<int, int>& origem, const std::pair<int, int>& destino) {

	std::vector<std::vector<bool>> visitados(linhas, std::vector<bool>(colunas, false)); // Rastreia os portos que já foram visitados durante a busca
	std::vector<std::vector<int>> menorCaminho(linhas, std::vector<int>(colunas, linhas * colunas)); // Armazena o custo acumulado de combustível até cada porto durante a busca

	int combustivel_gasto = 0;

	// Quando é encontrado um caminho com um custo menor que o previamente armazenado no menorCaminho, o valor é atualizado
	menorCaminho[origem.first][origem.second] = combustivel_gasto;

	// Soma o custo acumulado até o porto atual com a distância de Manhattan entre os pontos de partida e chegada
	// O combustivel_estimado é uma heurística utilizada no A* para estimar o custo restante até o destino
	int combustivel_estimado = combustivel_gasto + Manhattan(origem, destino);

	std::priority_queue<std::pair<int, std::pair<int, int>>> procurando; // É o equivalente ao "caderninho", contendo os portos a serem explorados
	procurando.emplace(std::make_pair(-combustivel_estimado, origem)); // Parâmetro negativo para obter uma fila em ordem crescente

	while (!procurando.empty()) { // Enquanto a fila de busca não estiver vazia


		std::pair<int, int> atual = procurando.top().second;
		procurando.pop();

		int i = atual.first;
		int j = atual.second;

		if (visitados[i][j] == true) { // Se o porto atual já foi visitado, continua
			continue;
		}

		visitados[i][j] = true; // Marca como visitado

		if (i == destino.first && j == destino.second) { // Se o porto atual é o destino...
			return menorCaminho[i][j]; // O custo acumulado de combustível até o porto atual é retornado
		}

		combustivel_gasto = menorCaminho[i][j] + 1; // incrementa combustivel

		// Contém os movimentos N, S, L, O para analizar os vizinhos
		std::vector<std::pair<int, int>> vizinhos = { {i - 1, j}, {i + 1, j}, {i, j + 1}, {i, j - 1} };

		for (auto& vizinho : vizinhos) { // Percorre pelos vizinhos

			int linha = vizinho.first;
			int coluna = vizinho.second;


			if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas && // Verifica se o vizinho está dentro dos limites do grafo e se não foi visitado anteriormente
				!visitados[linha][coluna] && grafo[linha][coluna] != '*') { // Verifica se o vizinho não é um *

				if (combustivel_gasto < menorCaminho[linha][coluna]) { // Verifica se esse caminho é melhor que o anterior

					menorCaminho[linha][coluna] = combustivel_gasto; // Atualiza o custo na matriz de combustível 
					combustivel_estimado = combustivel_gasto + Manhattan(vizinho, destino); // Calcula o custo estimado
					procurando.emplace(std::make_pair(-combustivel_estimado, vizinho)); // Insere o par para ser explorado posteriormente

				}
			}
		}
	}
	return -1; // Retorna -1 caso o destino não seja alcançável
}

void Dados::A_STAR() {

	int combustivel = 0;

	int porto_partida = 1;

	for (int i = 0; i < MAX_PORTOS; i++) {

		int porto_chegada = ((i + 1) % MAX_PORTOS) + 1; // 2 3 4 5 6 7 8 9 1
		std::cout << "\nPorto " << porto_partida << " para porto " << porto_chegada << std::endl;

		// começar pelo ponto de chegada para achar mais rapido portos obstruidos

		int distancia = CalculaDistancia(mapa_coordenadas[porto_chegada], mapa_coordenadas[porto_partida]);

		if (distancia == -1) {
			std::cout << " *** Porto de destino obstruido ***\n" << std::endl;
		}
		else {
			std::cout << "Distancia: " << distancia << std::endl;
			combustivel += distancia;
			porto_partida = porto_chegada;
		}
	}
	std::cout << "\nDistancia total percorrida: " << combustivel << std::endl;
}

int main() {

	auto start = std::chrono::high_resolution_clock::now();

	Dados dados("caso20.txt");

	if (dados.Mapa()) {
		dados.A_STAR();
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

	std::cout << std::fixed << std::setprecision(1) << duration.count() << " segundos." << std::endl;

	return 0;
}