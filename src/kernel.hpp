#pragma once
#include <array>
#include <vector>
#define NUMCORES 1
#define RAMMAX 5
#define DISKSPACE 10
#include "bootloader.hpp"
#include "cpu.hpp"
#include "disk.hpp"
#include "escalonador.hpp"
#include "process.hpp"
#include "ram.hpp"
#include <array>
// Acho que ja to ficando doidao
// Nada mais faz sentido
// Alguem me salva
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