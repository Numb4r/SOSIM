#include "shell.hpp"
#include "json.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
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
void Shell::execute() {
  std::thread t1(&Kernel::executeSystem, kernel);
  if (t1.joinable()) {
    t1.detach();
    // t1.join();
  }
}
void Shell::kill() {
  // this->kernel->reboot();
  exit(0);
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
    } else {
      printf("bash: %s : Comando nao encontrado\n", readline);
    }
  }
}