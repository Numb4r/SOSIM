#include "kernel.hpp"
#include "cpu.hpp"
#include "json.hpp"
#include "process.hpp"
Kernel::Kernel(const char *fileHardwarePath, const char *fileProcessPath)
    : isRunning(false), fileLoadPs(fileProcessPath) {

  nlohmann::json hrd = Bootloader().boot(fileHardwarePath);
  this->cpu = CPU(hrd["cores"]);
  this->ram = RAM(hrd["memsize"]);

  nlohmann::json ps = Bootloader().boot(fileProcessPath);
  const int pc = ps["processCount"];
  const int it = ps["interval"];
  this->escalonador = Escalonador(pc, it);
  for (auto &&i : ps["process"]) {
    escalonador.addProcessToList(i);
  }
}
std::string Kernel::ssCPU() { return this->cpu.snapshot(); }
std::string Kernel::ssMemory() { return this->ram.snapshot(); }
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
void Kernel::benchmarkCPU() {
  for (int i = 0; i < 99; i++) {
    Process ps;
    ps.setPID(i);
    ps.setQuantum(i);
    ps.changePriority(priorities::medium);

    this->cpu.loadPs(ps);
  }
}

void Kernel::benchmarkRAM() {
  for (int i = 0; i < 99; i++) {
    Process ps;
    ps.setPID(i);
    ps.setQuantum(i + 1);
    ps.changePriority(priorities::medium);

    this->ram.loadPs(ps);
  }
}