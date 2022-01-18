#pragma once
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <string>
using json = nlohmann::json;
class FileIO {
public:
  static json readJson(const char *filePath) {
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
      throw "erro ao abrir o arquivo";
    std::string line;
    std::stringstream fileBody;
    while (std::getline(file, line))
      fileBody << line;
    file.close();
    return json::parse(fileBody);
  }
  static void createJson(std::string nameFile, std::string text) {
    std::ofstream file;
    file.open(nameFile);
    if (!file.is_open())
      throw "error ao abrir o arquivo";
    file << text;
    file.close();
  }
};