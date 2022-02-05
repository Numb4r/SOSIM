#pragma once
#include "Log.hpp"
#include "cpu.hpp"
#include "disk.hpp"
#include "json.hpp"
#include "process.hpp"
#include "ram.hpp"
#include <list>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
enum schedulerPolicy { FIFO, LRU, MLQ };
class Escalonador {
  std::list<Process> FIFO(std::vector<Process> &listOfProcess);
  std::list<Process> LRU(std::vector<Process> &listOfProcess);
  std::list<Process> MLQ(std::vector<Process> &listOfProcess);
  enum schedulerPolicy policy;
  std::unordered_map<int, int> blockPsMap;
  std::map<std::string, enum resources> translateToEnum = {
      std::make_pair("cpu-bound", resources::cpu),
      std::make_pair("memory-bound", resources::ram),
      std::make_pair("io-bound", resources::disk)};

public:
  Escalonador(enum schedulerPolicy policy);
  Escalonador() = default;

  enum schedulerPolicy getPolicy() const;
  void applyPolicy(std::list<Process> &listOfProcess);
  void makeCycle(CPU &cpu, RAM &ram, Disk &disk,
                 std::list<Process> &listProcess, Log &log);
};