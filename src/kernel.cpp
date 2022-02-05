#include "kernel.hpp"
#include "cpu.hpp"
#include "json.hpp"
#include "process.hpp"
#include <chrono>
#include <iostream>
#include <thread>
Kernel::Kernel(const char *fileConfig, const char *fileProcessPath)
    : isRunning(false), fileLoadPs(fileProcessPath) {

  nlohmann::json config = Bootloader().boot(fileConfig);
  this->cpu = CPU(config["cores"]);
  this->ram = RAM(config["memsize"], config["pagsize"]);
  this->disk = Disk();
  this->escalonador = Escalonador(config["policy"]);
  this->log = Log(config["policy"]);
  nlohmann::json ps = Bootloader().boot(fileProcessPath);
  for (auto &&i : ps) {
    this->addProcessToList(i);
  }
}
std::string Kernel::ssCPU() { return this->cpu.snapshot(); }
std::string Kernel::ssMemory() { return this->ram.snapshot(); }
void Kernel::stopSystem() { this->isRunning = false; }
void Kernel::exportPs() { log.exportJson(); }
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
void Kernel::executeSystem() {
  this->isRunning = true;
  // printf("Running\n");
  while (this->isRunning) {
    if (this->listProcess.empty() || this->listProcess.size() <= 0) {
      this->isRunning = false;
      printf("Emtpy list");
    } else {
      escalonador.applyPolicy(this->listProcess);
      escalonador.makeCycle(cpu, ram, disk, this->listProcess, this->log);
    }
    std::vector<Process> arrayP{listProcess.begin(), listProcess.end()};
    for (unsigned long i = 0; i < arrayP.size(); i++) {
      if (arrayP.at(i).isProcessTerminated()) {
        // printf("erasing %d", arrayP.at(i).getPID());
        this->finishedProcess.push_back(arrayP.at(i));
        arrayP.erase(arrayP.begin() + i);
      }
    }
    listProcess = std::list<Process>{arrayP.begin(), arrayP.end()};
  }
}