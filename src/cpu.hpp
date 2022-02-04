#pragma once
#include "process.hpp"
#include <string>
#include <vector>
class Core {
public:
  int ncore;
  Process *ps;
  int timePassed = 0;
  // Process operator->() { return ps; }
  Core(int core, Process *ps) : ps(ps), ncore(core) {}
  Core(int ncore) : ncore(ncore), ps(nullptr) {}
};
class CPU {
  short numCores;

public:
  std::vector<Core> cores;
  CPU(const short numCores);
  CPU() = default;
  std::string snapshot();
  void reset();
  void loadPs(Process &ps);
};