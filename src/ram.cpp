#include "ram.hpp"
#include <sstream>
#include <vector>
RAM::RAM(const int memSize) : memSize(memSize) {}
void RAM::reset() {
  // std::swap(this->memory, std::vector<block>());
}
std::string RAM::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("memorySize":)" << this->memSize << R"(,"process":[)";
  for (auto &&i : this->memory) {
    ss << "{";
    ss << R"("pid":)" << i.ps.getPID() << R"(,"priority":)"
       << i.ps.getPriority() << R"(,"ts":)" << i.ps.getTimestamp()
       << R"(,"qt":)" << i.ps.getQuantum();
    ss << "}";
  }
  ss << "]}";
  return ss.str();
}
