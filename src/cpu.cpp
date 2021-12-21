#include "cpu.hpp"
#include "json.hpp"
#include <algorithm>

#include <iostream>
#include <sstream>
#include <vector>
CPU::CPU(const short numCores) : numCores(numCores) {}
void CPU::reset() {}
std::string CPU::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("numCores":)" << this->numCores << R"(,"process":[)";
  for (auto &&i : this->cores) {
    ss << "{";
    ss << R"("pid":)" << i.ps.getPID() << R"(,"priority":)"
       << i.ps.getPriority() << R"(,"ts":)" << i.ps.getTimestamp()
       << R"(,"qt":)" << i.ps.getQuantum();
    ss << "}";
  }

  ss << "]}";

  return ss.str();
}