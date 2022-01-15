#include "kernel.hpp"
#include "cpu.hpp"
#include "json.hpp"
#include "process.hpp"
#include <iostream>
Kernel::Kernel(const char *fileConfig, const char *fileProcessPath)
    : isRunning(false), fileLoadPs(fileProcessPath) {

  nlohmann::json config = Bootloader().boot(fileConfig);
  this->cpu = CPU(config["cores"]);
  this->ram = RAM(config["memsize"]);
  this->escalonador = Escalonador(config["policy"]);
  nlohmann::json ps = Bootloader().boot(fileProcessPath);
  for (auto &&i : ps) {
    this->addProcessToList(i);
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
}
void Kernel::addProcessToList(nlohmann::basic_json<> processInfo) {
  int processo = processInfo["processo"];
  int ciclos = processInfo["ciclos"];
  int max_quantum = processInfo["max_quantum"];
  enum resources init_type = this->translateToEnum.at(processInfo["init_type"]);
  enum priorities prioridade = processInfo["prioridade"];
  int timestamp = processInfo["timestamp"];
  this->listProcess.push_back(
      Process(processo, ciclos, max_quantum, init_type, prioridade, timestamp));
}
// TODO: Rescrever para separacao de politica de mecanismo do escalonador
//  Alem disso, consumir a lista toda em um ciclo, depois repetir ate que o
//  processo esteja finalizado
void Kernel::executeSystem() {
  while (!this->listProcess.empty()) {
    escalonador.applyPolicy(this->listProcess);
    escalonador.makeCycle(this->listProcess);
  }
}