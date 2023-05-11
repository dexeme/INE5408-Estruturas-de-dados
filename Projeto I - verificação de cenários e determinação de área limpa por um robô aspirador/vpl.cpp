#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <sstream>


std::string validaXml(const std::string& filename) {
    // Leitura do arquivo e inicilização da string do XML
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
                i += 2;
                std::string tagFechamento = "";
                while (xmlContent[i] != '>' && tagFechamento.length() < 20) {
                    tagFechamento += xmlContent[i];
                    i++;
                }
                if (pilhaTags.empty()) { // erro de validação: tag de fechamento sem correspondente de abertura
                    std::cout << "erro" << std::endl;
                    erro = true;
                } else { // tag de fechamento válida
                    // Se a ultima aberta for diferente da que esta fechando
                    if (pilhaTags.top() != tagFechamento) {
                        std::cout << "erro" << std::endl;
                        erro = true;
                    } else {
                        pilhaTags.pop();
                    }
                }
            } else { // tag de abertura
                i++;
                std::string tagAbertura = "";
                while (xmlContent[i] != '>' && tagAbertura.length() < 20) {
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

std::pair<int, int> extraiDimensoes(const std::string& xmlContent) {
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

std::pair<int, int> extraiPosicaoInicial(const std::string& xmlContent) {
    int alturaRobo = 0, larguraRobo = 0;
    size_t pos = xmlContent.find("<robo>");
    if (pos != std::string::npos) {
        pos = xmlContent.find("<x>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</x>", pos);
            if (endPos != std::string::npos) {
                std::string alturaStr = xmlContent.substr(pos + 3, endPos - pos - 3);
                alturaRobo = std::stoi(alturaStr);
            }
        }
        pos = xmlContent.find("<y>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</y>", pos);
            if (endPos != std::string::npos) {
                std::string larguraStr = xmlContent.substr(pos + 3, endPos - pos - 3);
                larguraRobo = std::stoi(larguraStr);
            }
        }
    }
    return std::make_pair(alturaRobo, larguraRobo);
}

std::string extraiNomeDoCenario(const std::string& xmlContent) {
    std::string nomeDoCenario;
    size_t pos = xmlContent.find("<cenario>");
    if (pos != std::string::npos) {
        pos = xmlContent.find("<nome>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</nome>", pos);
            if (endPos != std::string::npos) {
                nomeDoCenario = xmlContent.substr(pos + 6, endPos - pos - 6);
            }
        }
    }
    return nomeDoCenario;
}

int** matrizZerada(std::pair<int, int> dimensoes) {
    int altura = dimensoes.first;
    int largura = dimensoes.second;

    int** matriz = new int*[altura];
    for (int i = 0; i < altura; i++) {
        matriz[i] = new int[largura];
        for (int j = 0; j < largura; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

int** matrizOriginal(std::pair<int, int> dimensoes, const std::string& xmlContent) {
    int altura = dimensoes.first;
    int largura = dimensoes.second;
   
    int** matriz = new int*[altura];
    size_t pos = xmlContent.find("<matriz>") + 8;
    if (pos != std::string::npos) {
        for (int i = 0; i < altura; i++) {
            for (int j = 0; j < largura; j++) {
                matriz[i][j] = xmlContent[pos * i + j];
                std::cout << matriz[i][j] << "\n";
            }
        }
    }
    return matriz;
}

int main() {
    char xmlfilename[100];
    std::cin >> xmlfilename;
    std::string xmlContent = validaXml(xmlfilename);

    // std::pair<int, int> dimensoes = extraiDimensoes(xmlContent);
    // int altura = dimensoes.first;
    // int largura = dimensoes.second;

    // int** matrizOriginalZerada = matrizZerada(dimensoes);
   

    // std::pair<int, int> posicaoInicial = extraiPosicaoInicial(xmlContent);
    // int alturaInicial = posicaoInicial.first;
    // int larguraInicial = posicaoInicial.second;

    // std::string nomeCenario = extraiNomeDoCenario(xmlContent);

    return 0;

}

// -----------------------------------------------------------------------------------------

// TO-DO:

// 1. Achar uma forma de prevenir que o programa identifique quando a matriz no XML é inválida (exemplo das imagens)
// 2. Criar uma função semelhante a extraiDimensoes, mas que extraia a matriz do XML em forma de string (filtrar
// apenas os 0s e 1s)
// 3. Criar uma função semelhante a extraiDimensoes, mas que extraia a posição inicial (x,y) do robô OK
// 4. Criar uma função semelhante a extraiDimensoes, mas que extraia o nome da matriz (exemplo: "cenario-01"), OK
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