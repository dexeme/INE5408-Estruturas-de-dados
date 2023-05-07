#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <sstream>

// ------------------------------------ VALIDA O XML --------------------------------------------

std::string Valida_xml(const std::string& filename) {
    std::ifstream xmlFile(filename);
    if (!xmlFile.is_open()) {
        std::cout << "erro" << std::endl;
    }
    std::string xmlContent((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());
    xmlFile.close();
    std::stack<std::string> pilhaTags; // pilha para armazenar as tags
    bool erro = false; // flag para indicar erro de validação
    for (int i = 0; i < xmlContent.length() && !erro; i++) {
        if (xmlContent[i] == '<') {
            if (xmlContent[i + 1] == '/') { // tag de fechamento
                i++;
                std::string tagFechamento = "";
                while (xmlContent[i] != '>') {
                    tagFechamento += xmlContent[i];
                    i++;
                }
                if (pilhaTags.empty()) { // erro de validação: tag de fechamento sem correspondente de abertura
                    std::cout << "erro" << std::endl;
                    erro = true;
                } else { // tag de fechamento válida
                    pilhaTags.pop();
                }
            } else { // tag de abertura
                std::string tagAbertura = "";
                while (xmlContent[i] != '>') {
                    tagAbertura += xmlContent[i];
                    i++;
                }
                pilhaTags.push(tagAbertura);
            }
        }
    }
    if (!pilhaTags.empty()) { // erro de validação: tag de abertura sem correspondente de fechamento
        std::cout << "erro" << std::endl;
        erro = true;
        }
    return xmlContent;
}

// ---------------------------------------------------------------------------------------------

// ---------------------------- EXTRAI ALTURA E LARGURA DA MATRIZ ------------------------------

std::pair<int, int> ExtraiDimensoes(const std::string& xmlContent) {
    int altura = 0, largura = 0;
    size_t pos = xmlContent.find("<dimensoes>");
    if (pos != std::string::npos) {
        pos = xmlContent.find("<altura>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</altura>", pos);
            if (endPos != std::string::npos) {
                std::string alturaStr = xmlContent.substr(pos + 8, endPos - pos - 8);
                altura = std::stoi(alturaStr);
            }
        }
        pos = xmlContent.find("<largura>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</largura>", pos);
            if (endPos != std::string::npos) {
                std::string larguraStr = xmlContent.substr(pos + 9, endPos - pos - 9);
                largura = std::stoi(larguraStr);
            }
        }
    }
    return std::make_pair(altura, largura);
}

// ---------------------------------------------------------------------------------------

// ------------------ CRIA MATRIZ ZERADA DE MESMA DIMENSÃO ------------------------------------

std::string matrizZerada(std::pair<int, int> dimensoes) {
    int altura = dimensoes.first;
    int largura = dimensoes.second;
    std::string matriz = "";
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            matriz += "0";
        }
    }
    return matriz;
}

// ---------------------------------------------------------------------------------------


// ------------------------------------ SIMULA O ROBO ------------------------------------

int simulaRoboAspirador(const std::string& matrizContent, const std::string& matrizOriginalZerada, int& maxRegiaoConexa, int& altura, int& largura) {

    // inicializa matriz de visitados
    std::vector<std::vector<bool>> visitados(altura, std::vector<bool>(largura, false));

    // inicializa a fila com a posição inicial (0, 0)
    std::queue<std::pair<int, int>> fila;
    fila.push(std::make_pair(0, 0));
    visitados[0][0] = true;

    // inicializa variáveis de contagem
    int regiaoConexaAtual = 0;
    int maiorRegiaoConexa = 0;

    while (!fila.empty()) {
        // obtém a posição atual da fila
        auto posAtual = fila.front();
        fila.pop();
        regiaoConexaAtual++;

        // obtém as posições dos vizinhos
        int x = posAtual.first;
        int y = posAtual.second;
        std::vector<std::pair<int, int>> vizinhos = { {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1} };

        // percorre os vizinhos
        for (auto vizinho : vizinhos) {
            int x_vizinho = vizinho.first;
            int y_vizinho = vizinho.second;

            // verifica se a posição do vizinho é válida
            if (x_vizinho >= 0 && x_vizinho < altura && y_vizinho >= 0 && y_vizinho < largura) {
                // verifica se o vizinho não foi visitado e é um espaço para ser limpo
                if (!visitados[x_vizinho][y_vizinho] && matrizOriginalZerada[x_vizinho * largura + y_vizinho] == '1') {
                    // marca o vizinho como visitado e adiciona à fila
                    visitados[x_vizinho][y_vizinho] = true;
                    fila.push(std::make_pair(x_vizinho, y_vizinho));
                }
            }
        }
    }
    // atualiza a maior região conexa
    if (regiaoConexaAtual > maiorRegiaoConexa) {
        maiorRegiaoConexa = regiaoConexaAtual;
        maxRegiaoConexa = regiaoConexaAtual;
    }
    // printa a maior região conexa
    std::cout << maiorRegiaoConexa << std::endl;
    return regiaoConexaAtual;
}


// ---------------------------------------------------------------------------------------

// -------------------------------------- MAIN -------------------------------------------

int main() {
    char xmlfilename[100];
    std::cin >> xmlfilename;
    std::string xmlContent = Valida_xml(xmlfilename);
    std::pair<int, int> dimensoes = ExtraiDimensoes(xmlContent);
    int altura = dimensoes.first;
    int largura = dimensoes.second;
    std::string matrizOriginalZerada = matrizZerada(dimensoes);
    int maxRegiaoConexa = 0;
    simulaRoboAspirador(xmlContent, matrizOriginalZerada, maxRegiaoConexa, altura, largura);
    return 0;

}

// -----------------------------------------------------------------------------------------

// TO-DO:

// 1. Achar uma forma de prevenir que o programa identifique quando a matriz no XML é inválida (exemplo das imagens)
// 2. Criar uma função semelhante a ExtraiDimensoes, mas que extraia a matriz do XML em forma de string (filtrar
// apenas os 0s e 1s)
// 3. Criar uma função semelhante a ExtraiDimensoes, mas que extraia a posição inicial (x,y) do robô
// 4. Criar uma função semelhante a ExtraiDimensoes, mas que extraia o nome da matriz (exemplo: "cenario-01"), 
// para que o programa possa printar o nome da matriz na saída e também para que o programa salve a maior região
// conexa entre todos os cenários daquele cenario especifico (exemplo: "cenario-01 25")
// Exemplo de saída do programa:
//
// cenario-01 25
// cenario-02 107
// cenario-03 28
// cenario-04 0
// cenario-05 22
// cenario-06 174

// -----------------------------------------------------------------------------------------