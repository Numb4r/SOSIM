#pragma once
#include "json.hpp"
#include "process.hpp"
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>
enum schedulerPolicy { FIFO };
// TODO: List de processos iniciais deve ficar no Kernel, e nao no escalonador
// Escalonador deve ficar apenas com a parte da politica e mecanismo
class Escalonador {
  std::list<Process> FIFO(std::vector<Process> &listOfProcess);
  enum schedulerPolicy policy;
  std::map<std::string, enum resources> translateToEnum = {
      std::make_pair("cpu-bound", resources::cpu),
      std::make_pair("memory-bound", resources::ram),
      std::make_pair("io-bound", resources::disk)};

public:
  // std::vector<Process> deadProcess;

  Escalonador(enum schedulerPolicy policy);
  Escalonador() = default;
  // Process nextProcess();

  enum schedulerPolicy getPolicy() const;
  void makeCycle(std::list<Process> &listProcess);
  void applyPolicy(std::list<Process> &listOfProcess);
};