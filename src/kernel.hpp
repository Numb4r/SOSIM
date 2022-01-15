#pragma once
#include "bootloader.hpp"
#include "cpu.hpp"
#include "disk.hpp"
#include "escalonador.hpp"
#include "process.hpp"
#include "ram.hpp"
#include <queue>

class Kernel {
  CPU cpu;
  RAM ram;
  Disk disk;
  std::string fileLoadPs;
  Escalonador escalonador;
  bool isRunning;
  Process *ps;
  std::list<Process> listProcess;
  std::map<std::string, enum resources> translateToEnum = {
      std::make_pair("cpu-bound", resources::cpu),
      std::make_pair("memory-bound", resources::ram),
      std::make_pair("io-bound", resources::disk)};

public:
  Kernel(const char *fileConfig, const char *fileProcessPath);
  void executeSystem();
  void stopSystem();
  void addProcessToList(nlohmann::basic_json<> processInfo);
  std::string ssCPU();
  std::string ssMemory();
  void reboot();
};