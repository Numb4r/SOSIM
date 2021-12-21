#include "ram.hpp"
#include <sstream>
RAM::RAM(const int memSize) : memSize(memSize) {}
void RAM::reset() {
  // std::swap(this->memory, std::vector<block>());
}
std::string RAM::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("memorySize":)" << this->memSize << R"(,"process":[)";
  int comma = 0;
  for (auto &&i : this->memory) {
    ss << "{";
    ss << R"("pid":)" << i.ps.getPID() << R"(,"adr":)" << i.address
       << R"(,"pr":)" << i.ps.getPriority() << R"(,"ts":)"
       << i.ps.getTimestamp() << R"(,"qt":)" << i.ps.getQuantum();
    ss << "}";
    if (comma + 1 < this->memory.size())
      ss << ",";
    comma++;
  }
  ss << "]}";
  return ss.str();
}
static int address = 0;
void RAM::loadPs(Process ps) {
  if (address < this->memSize) {
    this->memory.push_back({address, ps});
    address++;
  }
}