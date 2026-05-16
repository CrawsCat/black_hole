#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <fstream>

static std::string readFile(const char* filepath) {
    std::string fileContent;
    std::string line;
    std::ifstream file(filepath);

    while(std::getline(file, line)) {
        fileContent += line + "\n";
    }

    return fileContent;
}

#endif