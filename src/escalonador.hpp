#pragma once
#include "json.hpp"
#include "process.hpp"
#include <list>
#include <queue>
#include <utility>
#define QUANTUMMAX 3
#include <vector>
#include <map>
#include <string>
class Escalonador {
  int processCountMax;
  int pc;
  // Lista de processos a serem criados
  std::list<Process> listProcess;
  // fila de processos criados e colocados como prontos para serem enviados para
  // execucao
  std::queue<Process> queueProcess;
  int intervalCreation;
  std::map<std::string,enum resources> translateToEnum={
    std::make_pair("cpu-bound", resources::cpu),
    std::make_pair("memory-bound", resources::ram),
    std::make_pair("io-bound", resources::disk)
  };

public:
  std::vector<Process> deadProcess;
  Escalonador(int processCountMax, const int intervalCreation);
  Escalonador();
  int getProcessCountMax();
  int getPc();
  void resetEscalonador(const int pc, const int it);
  Process nextProcess();

  void addProcessToList(nlohmann::basic_json<> processInfo);
  void createProcess(const int cycles);
  Process *getNextProcess(int cycles);
  bool isQueueEmpty();
  bool isListEmpty();
};