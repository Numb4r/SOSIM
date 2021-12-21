#pragma once
#include "process.hpp"
#include <string>
#include <vector>
class core {
public:
  Process ps;
  Process operator->() { return ps; }
  core(Process ps) : ps(ps) {}
};
class CPU {
  short numCores;

public:
  std::vector<core> cores;
  CPU(const short numCores);
  CPU() = default;
  std::string snapshot();
  void reset();
};