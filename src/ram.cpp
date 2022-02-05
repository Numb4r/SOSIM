#include "ram.hpp"
#include <cmath>
#include <sstream>
RAM::RAM(const int memSize, const int pagSize)
    : memSize(memSize), pagSize(pagSize) {
  for (int i = 0; i < memSize; i++) {
    this->memory.push_back(Block(i));
  }
}
void RAM::reset() {
  // std::swap(this->memory, std::vector<block>());
}
std::string RAM::snapshot() {
  std::stringstream ss;
  ss << "{";
  ss << R"("memorySize":)" << this->memSize << R"(,"pageSize":)"
     << this->pagSize << R"(,"process":[)";
  int comma = 0;
  for (int i = 0; i < memSize; i += pagSize) {
    if (this->memory[i].ps != nullptr) {
      ss << "{";
      ss << R"("pid":)" << this->memory[i].ps->getPID() << R"(,"adr":)" << '"'
         << this->memory[i].address << "-"
         << this->memory[i].address + this->pagSize << '"' << R"(,"pr":)"
         << this->memory[i].ps->getPriority() << R"(,"ts":)"
         << this->memory[i].ps->getTimestamp() << R"(,"qt":)"
         << this->memory[i].ps->getQuantum();
      ss << "}";
      if (comma + 1 < this->qntPagAlloc)
        ss << ",";
      comma++;
    }
  }
  ss << "]}";
  return ss.str();
}
void RAM::unloadPs(Process &ps) {
  std::vector<int> posMem = bitMap.find(ps.getPID())->second;
  printf("Unload %d\n", ps.getPID());
  for (auto &&i : posMem) {
    for (int j = i; j < (i + pagSize); j++) {
      printf("Removing from %d\n", j);
      this->memory[j].ps = nullptr;
    }
    this->qntPagAlloc--;
  }

  bitMap.erase(ps.getPID());
}

bool RAM::loadPs(Process &ps, int blockRequired) {
  std::vector<int> pagEmpty;
  int i = 0;
  const int pagRequired = ceil((float)blockRequired / (float)this->pagSize);
  int emptyPages{};
  // printf("%d %d %d\n", blockRequired, this->pagSize, pagRequired);
  while (i < memSize && emptyPages != pagRequired) {
    if (this->memory[i].ps == nullptr) {
      pagEmpty.push_back(i);
      emptyPages++;
    }
    i += (i + 1) * pagSize;
    // printf("Searching %d \t", emptyPages == pagRequired);
  }
  // printf("\n%zu\t%d", pagEmpty.size(), pagRequired);
  if (emptyPages >= pagRequired) {
    // printf("\nAlocando memoria para o processo %d: ", ps.getPID());
    for (auto &&i : pagEmpty) {
      // printf("\n%d\t %d ", i, pagSize);
      for (int j = i; j < i + pagSize; j++) {
        this->memory[j].ps = &ps;
      }
    }
    bitMap.insert({ps.getPID(), pagEmpty});
    this->qntPagAlloc += pagRequired;
    // printf("\n");
    return true;
  } else {
    // printf("\nNao alocando memoria,processo bloqueado");
    return false;
  }
}