#pragma once
#include "kernel.hpp"
class Shell {
  Kernel *kernel;
  void help();
  void meminfo();
  void cpuinfo();
  void queueschell();
  void execute();
  void kill();
  void membench();
  void cpubench();
  void stopSystem();

public:
  Shell(Kernel *kernel);
  void loop();
};