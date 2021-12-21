#pragma once
#include "process.hpp"
#include <string>
#include <vector>
class core {
public:
  int ncore;
  Process ps;
  Process operator->() { return ps; }
  core(int core, Process ps) : ps(ps), ncore(core) {}
};
class CPU {
  short numCores;

public:
  std::vector<core> cores;
  CPU(const short numCores);
  CPU() = default;
  std::string snapshot();
  void reset();
  void loadPs(Process ps);
};