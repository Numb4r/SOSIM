#include "kernel.hpp"
#include "shell.hpp"
int main(const int argc, char **argv) {
  Kernel kernel("hardware.json", "processos.json");
  Shell shell(&kernel);
  shell.loop();
  return 0;
}