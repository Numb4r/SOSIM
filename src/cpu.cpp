#include "cpu.hpp"
#include <sstream>
CPU::CPU(const short numCores) : numCores(numCores) {}
void CPU::reset() {}
std::string CPU::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("numCores":)" << this->numCores << R"(,"process":[)";
  int comma = 0;
  for (auto &&i : this->cores) {
    ss << "{";
    ss << R"("pid":)" << i.ps.getPID() << R"(,"cr":)" << i.ncore << R"(,"pr":)"
       << i.ps.getPriority() << R"(,"ts":)" << i.ps.getTimestamp()
       << R"(,"qt":)" << i.ps.getQuantum();
    ss << "}";
    if (comma + 1 < this->cores.size())
      ss << ",";
    comma++;
  }

  ss << "]}";

  return ss.str();
}
static int core = 0;
void CPU::loadPs(Process ps) {
  if (core < this->numCores) {
    this->cores.push_back({core, ps});
    core++;
  }
}