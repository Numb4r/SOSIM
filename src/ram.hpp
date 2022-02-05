#pragma once
#include "process.hpp"
#include <string>
#include <unordered_map>
#include <vector>
class Block {
public:
  int address;
  Process *ps;
  // Process operator->() { return ps; }
  Block(int address) : address(address), ps(nullptr) {}
  Block() = default;
};
class RAM {
  int memSize;
  int pagSize;
  int qntPagAlloc{};
  std::vector<Block> memory;
  std::unordered_map<int, std::vector<int>> bitMap;

public:
  void reset();
  RAM(const int memSize, const int pagSize);
  RAM() = default;
  std::string snapshot();
  bool loadPs(Process &ps, int blockRequired);
  void unloadPs(Process &ps);
};