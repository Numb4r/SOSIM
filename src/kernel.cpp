#include "kernel.hpp"
#include "json.hpp"
#include "process.hpp"
Kernel::Kernel(const char *fileProcessPath)
    : cpu(CPU(NUMCORES)), ram(RAM(RAMMAX)), disk(Disk()), isRunning(false),
      fileLoadPs(fileProcessPath) {

  nlohmann::json j = Bootloader().boot(fileProcessPath);
  const int pc = j["processCount"];
  const int it = j["interval"];
  this->escalonador = Escalonador(pc, it);
  for (auto &&i : j["process"]) {
    escalonador.addProcessToList(i);
  }
}
void Kernel::stopSystem() { this->isRunning = false; }
void Kernel::reboot() {
  this->stopSystem();
  this->cpu.reset();
  this->ram.reset();
  this->disk = Disk();
  nlohmann::json j = Bootloader().boot(fileLoadPs.c_str());

  const int pc = j["processCount"];
  const int it = j["interval"];

  this->escalonador.resetEscalonador(pc, it);
  printf("%d %d", pc, it);
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
}