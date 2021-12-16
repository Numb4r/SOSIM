#pragma once
#include "json.hpp"
#include <fstream>
#include <sstream>
using json = nlohmann::json;
class FileIO{
    public:
    static json readJson(const char* filePath){
        std::ifstream file ;
        file.open(filePath);
        if(!file.is_open())
            throw "erro ao abrir o arquivo";
        std::string line;
        std::stringstream fileBody;
        while(std::getline(file,line))
            fileBody << line;
        file.close();
        return json::parse(fileBody);
    }

};