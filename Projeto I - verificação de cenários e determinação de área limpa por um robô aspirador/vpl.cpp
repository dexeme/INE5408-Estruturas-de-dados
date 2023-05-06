#include <iostream>
#include <fstream>
#include <string>
#include <stack>

int main() {
    char xmlfilename[100];
    std::string xmlContent; // conteúdo do arquivo XML
    // entrada do nome do arquivo
    std::cin >> xmlfilename;

    // leitura do arquivo XML
    std::ifstream xmlFile(xmlfilename);
    if (xmlFile.is_open()) {
        std::string linha;
        while (getline(xmlFile, linha)) {
            xmlContent += linha;
        }
        xmlFile.close();
    } else {
        std::cout << "erro" << std::endl;
        return 0;
    }

    // validação do arquivo XML
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
    if (!pilhaTags.empty() && !erro) { // erro de validação: tag de abertura sem correspondente de fechamento
        std::cout << "erro" << std::endl;
        erro = true;
    }
    if (!erro) {
        std::cout << xmlContent << std::endl;
        // TODO: processar a matriz binária e imprimir o resultado
    }

    return 0;
}