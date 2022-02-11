#include "shell.hpp"
#include "json.hpp"
#include <algorithm>
#include <curses.h>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>
Shell::Shell(Kernel *kernel) : kernel(kernel) {}
// TODO: Reescrever shell para uma forma mais dinamica, facilitando a criacao de
// comandos
void Shell::kill() { exit(0); }
void Shell::stopSystem() { this->kernel->stopSystem(); }

void Shell::help() {
  printf("help:\tMostra comandos do shell\n");
  printf("meminfo:\tInformacoes sobre a memoria RAM.\n");
  printf("cpuinfo:\tInformacoes sobre o CPU.\n");
  printf("queueschell:\tDetalha quais processos estão sendo gerenciados pelo "
         "sistema\n");
  printf("execute:\tExecuta a fila de processos definida\n");
  printf("kill -9:\tFinaliza a execução do sistema operacional\n");
  printf("stop:\tInterrompe e execucao do sistema");
  printf("export ps:\t Exporta informacoes sobre os processos");
}
void Shell::meminfo() {

  nlohmann::json j = json::parse(kernel->ssMemory());
  int memSize = j["memorySize"];
  int pagSize = j["pageSize"];
  auto vec = j["process"];

  printf("[");
  int qntPagAlloc{};
  for (auto &&i : vec) {
    qntPagAlloc++;
  }
  for (int i = 0; i < memSize; i += pagSize) {
    if (i < qntPagAlloc)
      printf("///////");
    else
      printf("\t");
    if (i + 1 < memSize)
      printf("|");
  }
  std::string s;
  printf("]\n");
  std::vector<std::string> info = {"pid", "adr", "pr", "ts", "qt"};
  printf("{");
  for (auto &&i : info) {
    printf("\n%s : ", i.c_str());
    for (auto &&j : vec) {
      std::cout << j[i] << "\t";
    }
  }
  printf("\n}\n");
}
void Shell::cpuinfo() {
  nlohmann::json j = json::parse(kernel->ssCPU());

  int cores = j["numCores"];
  auto vec = j["process"];
  std::vector<int> cr;
  for (auto &&i : vec) {
    cr.push_back(i["cr"]);
  }
  for (int i = 0; i < cores; i++) {
    if (std::find(cr.begin(), cr.end(), i) != cr.end())
      printf("[///////////////////////]");
    else
      printf("[\t\t\t]");
  }
  std::string s;

  std::vector<std::string> info = {"pid", "pr", "ts", "qt"};
  printf("\n{");
  for (auto &&i : info) {
    printf("\n%s : ", i.c_str());
    for (auto &&j : vec) {
      std::cout << j[i] << "\t";
    }
  }
  printf("\n}\n");
}
void Shell::queueschell() {
  nlohmann::json json = json::parse(kernel->ssQueuePs());
  std::vector<std::string> info = {"pid", "pr",  "ts", "qt",
                                   "rs",  "cly", "st", "m_qt"};
  auto process = json["process"];
  for (auto &&i : info) {
    printf("%s ", i.c_str());
  }
  printf("\n");
  for (auto &&i : process) {
    for (auto &&j : info) {
      std::cout << i[j] << " ";
    }
    printf("\n");
  }
}
void Shell::execute() {
  std::thread t1(&Kernel::executeSystem, kernel);
  if (t1.joinable()) {
    t1.detach();
    // t1.join();
  }
}
void Shell::exportPs() { this->kernel->exportPs(); }
void Shell::loop() {
  while (true) {
    // char readline[256];
    std::string s;
    printf("~$ ");
    try {
      std::getline(std::cin, s);
    } catch (std::istream::failure e) {
      std::cout << "[Error] " << e.what() << "\n";
    }

    if (std::strcmp(s.c_str(), "help") == 0) {
      help();
    } else if (std::strcmp(s.c_str(), "meminfo") == 0) {
      meminfo();
    } else if (std::strcmp(s.c_str(), "cpuinfo") == 0) {
      cpuinfo();
    } else if (std::strcmp(s.c_str(), "queueschell") == 0) {
      queueschell();
    } else if (std::strcmp(s.c_str(), "execute") == 0) {
      execute();
    } else if (std::strcmp(s.c_str(), "kill -9") == 0) {
      kill();
    } else if (std::strcmp(s.c_str(), "stop") == 0) {
      stopSystem();
    } else if (std::strcmp(s.c_str(), "export ps") == 0) {
      exportPs();
    } else {
      printf("bash: %s : Comando nao encontrado\n", s.c_str());
    }
  }
}