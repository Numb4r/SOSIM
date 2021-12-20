#pragma once
#include "process.hpp"
#include <list>
class CPU {
  const short numCores;
  std::list<Process> cores;

public:
  CPU(const short numCores);
  void reset();
};