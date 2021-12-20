#include "kernel.hpp"
#include "json.hpp"
#include "process.hpp"
Kernel::Kernel(const char *fileProcessPath)
    : cpu(CPU(NUMCORES)), ram(RAM(RAMMAX)), disk(Disk()), isRunning(false) {

  nlohmann::json j = Bootloader().boot(fileProcessPath);
  const int pc = j["processCount"];
  const int it = j["interval"];
  this->escalonador = Escalonador(pc, it);
  for (auto &&i : j["process"]) {
    escalonador.addProcessToList(i);
  }
}
void Kernel::executeSystem() {
  this->isRunning = true;
  int cycles = 0;
  while (this->isRunning) {
    this->escalonador.createProcess(cycles);

    this->ps = this->escalonador.getNextProcess(cycles);

    printf("\n[%d]:", cycles);
    if (ps) {
      ps->makeCycle();
      printf("%d q:%d ts:%d cycles:%d", ps->getPID(), ps->getQuantum(),
             ps->getTimestamp(), ps->getCycles());
    } else if (this->escalonador.isListEmpty()) {
      this->isRunning = false;
      break;
    }

    cycles++;
  }
  // for (auto &&i : this->escalonador.deadProcess) {
  //   printf("\n%d %d %d %d", i.getPID(), i.isTerminated(), i.getQuantum(),
  //          i.getTimestamp());
  // }
}