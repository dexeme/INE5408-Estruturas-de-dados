#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <sstream>
#include <algorithm>

// Retorna o conteúdo do XML, caso ele seja válido. Se não, imprime "erro" e
// retorna uma string vazia.
std::string validateXML(const std::string& filename) {
    // Leitura do arquivo e inicilização da string do XML
    std::ifstream xmlFile(filename);
    if (!xmlFile.is_open()) {
        std::cout << "erro" << std::endl;
        return "";
    }
    std::string xmlContent((std::istreambuf_iterator<char>(xmlFile)), std::istreambuf_iterator<char>());
    xmlFile.close();

    std::stack<std::string> tagStack; // pilha para armazenar as tags
    for (int i = 0; i < xmlContent.length(); i++) {
        if (xmlContent[i] == '<') {
            if (xmlContent[i + 1] == '/') { // tag de fechamento
                i += 2;
                std::string closingTag = "";
                while (xmlContent[i] != '>' && closingTag.length() < 20) {
                    closingTag += xmlContent[i];
                    i++;
                }
                if (tagStack.empty()) { // erro de validação: tag de fechamento sem correspondente de abertura
                    std::cout << "erro" << std::endl;
                    return "";
                } else { // tag de fechamento válida
                    // Se a ultima aberta for diferente da que esta fechando
                    if (tagStack.top() != closingTag) {
                        std::cout << "erro" << std::endl;
                        return "";
                    } else {
                        tagStack.pop();
                    }
                }
            } else { // tag de abertura
                i++;
                std::string openingTag = "";
                while (xmlContent[i] != '>' && openingTag.length() < 20) {
                    openingTag += xmlContent[i];
                    i++;
                }
                tagStack.push(openingTag);
            }
        }
    }
    if (!tagStack.empty()) { // erro de validação: tag de abertura sem correspondente de fechamento
        std::cout << "erro" << std::endl;
        return "";
        }
    return xmlContent;
}


// Retorna um par int<x, y> contendo as dimensões da matriz do cenário provido.
std::pair<int, int> getMatrixDimensions(const std::string& xmlContent, std::size_t casePos) {
    int height = 0, width = 0;
    size_t pos = xmlContent.find("<dimensoes>", casePos);
    if (pos != std::string::npos) {
        pos = xmlContent.find("<altura>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</altura>", pos);
            if (endPos != std::string::npos) {
                std::string heightStr = xmlContent.substr(pos + 8, endPos - pos - 8);
                height = std::stoi(heightStr);
            }
        }
        pos = xmlContent.find("<largura>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</largura>", pos);
            if (endPos != std::string::npos) {
                std::string widthStr = xmlContent.substr(pos + 9, endPos - pos - 9);
                width = std::stoi(widthStr);
            }
        }
    }
    return std::make_pair(height, width);
}


// Extrai a posição inicial (x, y) do robo, invertendo a relação que o XML
// considera: (x) como deslocamento vertical e (y) como horizontal.
//
// Retorna um par int<x, y> contendo a posição inicial do robô dada a posição
// do cenário no XML.
std::pair<int, int> getBotPosition(const std::string& xmlContent, std::size_t casePos) {
    int bot_x = 0, bot_y = 0;
    size_t pos = xmlContent.find("<robo>", casePos);
    if (pos != std::string::npos) {
        pos = xmlContent.find("<x>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</x>", pos);
            if (endPos != std::string::npos) {
                std::string yStr = xmlContent.substr(pos + 3, endPos - pos - 3);
                bot_y = std::stoi(yStr);
            }
        }
        pos = xmlContent.find("<y>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</y>", pos);
            if (endPos != std::string::npos) {
                std::string xStr = xmlContent.substr(pos + 3, endPos - pos - 3);
                bot_x = std::stoi(xStr);
            }
        }
    }
    return std::make_pair(bot_x, bot_y);
}


// Retorna o nome do cenário dada sua posição no XML.
std::string getCaseName(const std::string& xmlContent, std::size_t casePos) {
    std::string caseName;
    size_t pos = xmlContent.find("<cenario>", casePos);
    if (pos != std::string::npos) {
        pos = xmlContent.find("<nome>", pos);
        if (pos != std::string::npos) {
            size_t endPos = xmlContent.find("</nome>", pos);
            if (endPos != std::string::npos) {
                caseName = xmlContent.substr(pos + 6, endPos - pos - 6);
            }
        }
    }
    return caseName;
}


// Retorna uma matriz de inteiros zerada conforme as dimensões providas.
int** getClearMatrix(std::pair<int, int> dimensions) {
    int height = dimensions.first;
    int width = dimensions.second;

    int** matrix = new int*[height];
    for (int i = 0; i < height; i++) {
        matrix[i] = new int[width];
        for (int j = 0; j < width; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}


int** getCaseMatrix(const std::string& xmlContent, std::pair<int, int> dimensions, std::size_t casePos) {
    int height = dimensions.first;
    int width = dimensions.second;
    int** matrix = new int*[height];

    size_t pos = xmlContent.find("<matriz>", casePos);
    std::string matrixStr;
    if (pos != std::string::npos) {
        std::size_t endPos = xmlContent.find("</matriz>");
        if (endPos != std::string::npos) {
            matrixStr = xmlContent.substr(pos+9, endPos-(pos+9));
        }
    }
    matrixStr.erase(std::remove(matrixStr.begin(), matrixStr.end(), '\n'),
                    matrixStr.end());

    for (int i = 0; i < height; i++) {
        matrix[i] = new int[width];
        for (int j = 0; j < width; j++) {
            char cell = matrixStr[i * width + j];
            if (cell == '0' || cell == '1') {
                int intCell = cell - '0';
                matrix[i][j] = intCell;
            }
        }
    }
    return matrix;
}


// Funcao auxiliar que retorna o número de cenários de um XML.
int getNumOfCases(std::string xmlContent) {
    int count = 0;
    std::size_t pos = 0;
    std::string s = xmlContent;
    std::string target = "<cenario>";
    while ((pos = s.find(target, pos )) != std::string::npos) {
            ++count;
            pos += target.length();
    }
    return count;
}


// Retorna um vetor contendo a posição inicial de cada tag cenario dado um XML.
std::size_t* getCasesPositions(std::string xmlContent, int numOfCases) {
    std::size_t* casesPos = new std::size_t[numOfCases];

    std::string s = xmlContent;
    std::size_t offset = 0;

    for (int i = 0; i < numOfCases; i++) {
        std::size_t startPos = s.find("<cenario>", offset);
        std::size_t endPos = s.find("</cenario>", offset);
        offset = endPos + 10;

        casesPos[i] = startPos;
    }
    return casesPos;
}


// Imprime a matriz resultado para cada iteração da rotina do robô.
void debug(int** clearMatrix, int height, int width) {
    for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                std::cout << clearMatrix[i][j];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
}


// Retorna o número de células limpas pelo robô.
int runBot(int** caseMatrix, std::pair<int, int> matrixDimensions, std::pair<int, int> botPos) {

    int **clearMatrix = getClearMatrix(matrixDimensions);
    std::queue<std::pair<int, int>> cellsQueue;

    int bot_x = botPos.first;
    int bot_y = botPos.second;
    int heigt = matrixDimensions.first;
    int width = matrixDimensions.second;
    int total = 0;

    if (caseMatrix[bot_y][bot_x] != 1) {
        return 0;
    }

    ++total;
    cellsQueue.push(std::make_pair(bot_y, bot_x));

    do {
        clearMatrix[bot_y][bot_x] = 1;
        
        int n1 = bot_y - 1 < 0 ? 0      : caseMatrix[bot_y - 1][bot_x];
        int n2 = bot_x + 1 >= width ? 0 : caseMatrix[bot_y][bot_x + 1];
        int n3 = bot_y + 1 >= heigt ? 0 : caseMatrix[bot_y + 1][bot_x];
        int n4 = bot_x - 1 < 0 ? 0      : caseMatrix[bot_y][bot_x - 1];

        if (n1 && !clearMatrix[bot_y - 1][bot_x])  {
            clearMatrix[bot_y - 1][bot_x] = 1;
            ++total;
            cellsQueue.push(std::make_pair(bot_y - 1, bot_x));
        }
        if (n2 && !clearMatrix[bot_y][bot_x + 1]) {
            clearMatrix[bot_y][bot_x + 1] = 1;
            ++total;
            cellsQueue.push(std::make_pair(bot_y, bot_x + 1));
        }
        if (n3 && !clearMatrix[bot_y + 1][bot_x]) {
            clearMatrix[bot_y + 1][bot_x] = 1;
            ++total;
            cellsQueue.push(std::make_pair(bot_y + 1, bot_x));
        }
        if (n4 && !clearMatrix[bot_y][bot_x - 1]) {
            clearMatrix[bot_y][bot_x - 1] = 1;
            ++total;
            cellsQueue.push(std::make_pair(bot_y, bot_x - 1));
        }

        cellsQueue.pop();
        bot_y = cellsQueue.front().first;
        bot_x = cellsQueue.front().second;

    } while(!cellsQueue.empty());

    return total;
}


// Inicializa a rotina de limpeza do robô.
void initBotRoutine(std::string xmlContent, std::size_t casePos) {
    
    std::pair<int, int> dimensions = getMatrixDimensions(xmlContent, casePos);
    int **caseMatrix = getCaseMatrix(xmlContent, dimensions, casePos);
    std::pair<int, int> botPos = getBotPosition(xmlContent, casePos);

    int numOfClearedCells = runBot(caseMatrix, dimensions, botPos);

    std::string caseName = getCaseName(xmlContent, casePos);
    std::cout << caseName << " " << numOfClearedCells << "\n";
}


int main(int argc, char** argv) {
    char xmlfilename[100];
    std::cin >> xmlfilename;
    std::string xmlContent = validateXML(xmlfilename);

    int numOfCases = getNumOfCases(xmlContent);
    std::size_t* casePos = getCasesPositions(xmlContent, numOfCases);

    for (int i = 0; i < numOfCases; i++) {
        initBotRoutine(xmlContent, casePos[i]);
    }

    return 0;
}
