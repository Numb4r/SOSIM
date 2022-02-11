#include "shell.hpp"
#include "json.hpp"
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
Shell::Shell(Kernel *kernel) : kernel(kernel) {}
// TODO: Reescrever shell para uma forma mais dinamica, facilitando a criacao de
// comandos
const std::map<int, std::string> mapToPriorities = {
    {4, "realtime"}, {3, "high"}, {2, "medium"}, {1, "low"}, {0, "zero"}};
const std::map<int, std::string> mapToState = {
    {0, "criado"},    {1, "pronto"},     {2, "execucao"},
    {3, "bloqueado"}, {4, "finalizado"}, {5, "esperando"}};
const std::map<int, std::string> mapToResources = {
    {0, "CPU"}, {1, "RAM"}, {2, "DISK"}};

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
    std::cout << "\n" << std::setw(3) << i << ": ";
    for (auto &&j : vec) {
      if (std::strcmp(i.c_str(), "adr") == 0) {
        std::string s = j[i];
        std::cout << std::setw(8) << s;
      } else {
        int s = j[i];
        if (std::strcmp(i.c_str(), "pr") == 0) {
          std::cout << std::setw(5) << mapToPriorities.at(s);
        } else {
          std::cout << std::setw(4) << s;
        }
      }
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
    std::cout << "\n" << std::setw(3) << i << ": ";
    for (auto &&j : vec) {
      int s = j[i];
      if (std::strcmp(i.c_str(), "pr") == 0) {
        std::cout << std::setw(5) << mapToPriorities.at(s);
      } else {
        std::cout << std::setw(4) << s;
      }
    }
  }
  printf("\n}\n");
}
void Shell::queueschell() {
  nlohmann::json json = json::parse(kernel->ssQueuePs());
  std::vector<std::string> info = {"pid", "pr", "ts", "rs",
                                   "cly", "st", "qt", "m_qt"};
  // std::cout.width(5);
  auto process = json["process"];
  for (auto &&i : info) {
    if (std::strcmp(i.c_str(), "pr") == 0) {
      std::cout << std::setw(10);
    } else if (std::strcmp(i.c_str(), "rs") == 0) {
      std::cout << std::setw(5);
    } else if (std::strcmp(i.c_str(), "st") == 0) {
      std::cout << std::setw(10);
    } else {
      std::cout << std::setw(5);
    }
    std::cout << i;
  }
  printf("\n");
  for (auto &&i : process) {
    for (auto &&j : info) {
      int s = i[j];
      if (std::strcmp(j.c_str(), "pr") == 0) {
        std::cout << std::setw(10) << mapToPriorities.at(s);
      } else if (std::strcmp(j.c_str(), "rs") == 0) {
        std::cout << std::setw(5) << mapToResources.at(s);
      } else if (std::strcmp(j.c_str(), "st") == 0) {
        std::cout << std::setw(10) << mapToState.at(s);
      } else {
        std::cout << std::setw(5) << s;
      }
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