#include "shell.hpp"
#include "json.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
Shell::Shell(Kernel *kernel) : kernel(kernel) {}

void Shell::help() {
  printf("help:\tMostra comandos do shell\n");
  printf("meminfo:\tInformacoes sobre a memoria RAM.\n");
  printf("cpuinfo:\tInformacoes sobre o CPU.\n");
  printf("queueschell:\tDetalha quais processos estão sendo gerenciados pelo "
         "sistema\n");
  printf("execute:\tExecuta a fila de processos definida\n");
  printf("kill -9:\tFinaliza a execução do sistema operacional\n");
  printf("membench:\tExecuta um full load na RAM para fins de teste\n");
  printf("cpubench:\tExecuta um full load no CPU para fins de teste\n");
}
void Shell::meminfo() {
  nlohmann::json j = json::parse(kernel->ssMemory());
  int memSize = j["memorySize"];
  auto vec = j["process"];

  printf("[");
  std::vector<int> address;
  for (auto &&i : vec) {
    address.push_back(i["adr"]);
  }

  for (int i = 0; i < memSize; i++) {
    if (std::find(address.begin(), address.end(), i) != address.end())
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
void Shell::queueschell() {}
void Shell::execute() { this->kernel->executeSystem(); }
void Shell::cpubench() { this->kernel->benchmarkCPU(); }
void Shell::membench() { this->kernel->benchmarkRAM(); }
void Shell::kill() {
  // this->kernel->reboot();
}
void Shell::loop() {
  while (true) {

    char readline[256];
    printf("~$ ");
    std::cin.getline(readline, 256);
    if (std::strcmp(readline, "help") == 0) {
      help();
    } else if (std::strcmp(readline, "meminfo") == 0) {
      meminfo();
    } else if (std::strcmp(readline, "cpuinfo") == 0) {
      cpuinfo();
    } else if (std::strcmp(readline, "queueschell") == 0) {
      queueschell();
    } else if (std::strcmp(readline, "execute") == 0) {
      execute();
    } else if (std::strcmp(readline, "kill -9") == 0) {
      kill();
    } else if (std::strcmp(readline, "cpubench") == 0) {
      this->cpubench();
    } else if (std::strcmp(readline, "membench") == 0) {
      this->membench();
    } else {
      printf("bash: %s : Comando nao encontrado\n", readline);
    }
  }
}