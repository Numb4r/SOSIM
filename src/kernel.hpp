#pragma once
#include "bootloader.hpp"
#include "cpu.hpp"
#include "disk.hpp"
#include "escalonador.hpp"
#include "process.hpp"
#include "ram.hpp"

class Kernel {
  CPU cpu;
  RAM ram;
  Disk disk;
  std::string fileLoadPs;
  Escalonador escalonador;
  bool isRunning;
  Process *ps;

public:
  Kernel(const char *fileHardwarePath, const char *fileProcessPath);
  void executeSystem();
  void stopSystem();
  std::string ssCPU();
  std::string ssMemory();
  void reboot();
};