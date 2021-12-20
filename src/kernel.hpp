#pragma once
#include <array>
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
  Escalonador escalonador;
  bool isRunning;
  Process *ps;

public:
  Kernel(const char *fileProcessPath);
  void executeSystem();
  void stopSystem();
};