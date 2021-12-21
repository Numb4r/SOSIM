#pragma once
#include "process.hpp"
#include <string>
#include <vector>
class block {
public:
  int address;
  Process ps;
  Process operator->() { return ps; }
  block(int address, Process ps) : address(address), ps(ps) {}
};
class RAM {
  int memSize;
  std::vector<block> memory;

public:
  void reset();
  RAM(const int memSize);
  RAM() = default;
  std::string snapshot();
  void loadPs(Process ps);
};