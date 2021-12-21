#pragma once
#include "process.hpp"
#include <string>
#include <vector>
class block {
public:
  Process ps;
  Process operator->() { return ps; }
  block(Process ps) : ps(ps) {}
};
class RAM {
  int memSize;
  std::vector<block> memory;

public:
  void reset();
  RAM(const int memSize);
  RAM() = default;
  std::string snapshot();
};