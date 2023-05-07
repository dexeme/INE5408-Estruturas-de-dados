#include <iostream>
#include <fstream>
#include <string>
#include <stack>

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


// -------------------------------------- MAIN -------------------------------------------

int main() {
    char xmlfilename[100];
    std::cin >> xmlfilename;
    std::string xmlContent = Valida_xml(xmlfilename);
    std::pair<int, int> dimensoes = ExtraiDimensoes(xmlContent);
    int altura = dimensoes.first;
    int largura = dimensoes.second;
    return 0;

}

// -----------------------------------------------------------------------------------------

