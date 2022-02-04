#include "cpu.hpp"
#include <sstream>
CPU::CPU(const short numCores) : numCores(numCores) {
  for (int i = 0; i < numCores; i++) {
    this->cores.push_back(Core(i));
  }
}
void CPU::reset() {}
std::string CPU::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("numCores":)" << this->numCores << R"(,"process":[)";
  int comma = 0;
  for (auto &&i : this->cores) {
    if (i.ps != nullptr) {
      ss << "{";
      ss << R"("pid":)" << i.ps->getPID() << R"(,"cr":)" << i.ncore
         << R"(,"pr":)" << i.ps->getPriority() << R"(,"ts":)"
         << i.ps->getTimestamp() << R"(,"qt":)" << i.ps->getQuantum();
      ss << "}";
      if (comma + 1 < this->cores.size())
        ss << ",";
      comma++;
    }
  }

  ss << "]}";

  return ss.str();
}

void CPU::loadPs(Process &ps) {
  if (numCores == 1) {
    this->cores[0].ps = &ps;
  }
  // int olderPs = 0;
  // for (int i = 0; i < numCores; i++) {
  //   if (this->cores[i].ps == nullptr) {
  //     this->cores[i].ps = &ps;
  //     return;
  //   } else {
  //     if (this->cores[i].timePassed > this->cores[olderPs].timePassed)
  //       olderPs = i;
  //   }
  // }
  // this->cores[olderPs].ps = &ps;
}