#include <iostream>
#include <fstream>
#include <string>
#include <stack>

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

                } else if (pilhaTags.top() != tagFechamento) { // erro de validação: tag de fechamento com nome diferente da última tag de abertura
                    std::cout << "erro" << std::endl;
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
    }

    return xmlContent;
}

        

int main() {
    char xmlfilename[100];
    std::string xmlContent;
    std::cin >> xmlfilename;
    xmlContent = Valida_xml(xmlfilename);
    return 0;

}