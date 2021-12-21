#include "shell.hpp"
#include "json.hpp"
#include <functional>
#include <iostream>
#include <stdio.h>
#include <string>
Shell::Shell(Kernel *kernel) : kernel(kernel) {}

void Shell::help() {
  printf("help:\tDemonstra quais comandos podem ser executados pelo shell "
         "desenvolvido\n");
  printf("meminfo:\tDetalha quantas unidades de memória so sistema possui, "
         "quantas estão livres e quantas estão alocadas e para quais "
         "processos.\n");
  printf("cpuinfo:\tDetalha qual processo está em execução, qual o timestamp e "
         "quantum desse.\n");
  printf("queueschell:\tDetalha quais processos estão sendo gerenciados pelo "
         "seu sistema, quais estão em estado de pronto, bloqueado, execução "
         "e/ou sendo criados e finalizados.\n");
  printf("execute:\tExecuta a fila de processos definida conforme configuração "
         "prévia.\n");
  printf("kill -9:\tFinaliza a execução do sistema operacional, voltando o "
         "mesmo para o estado inicial, em que os processos estariam ainda em "
         "faze de criação inicial.\n");
}
void Shell::meminfo() { nlohmann::json j = json::parse(kernel->ssMemory()); }
void Shell::cpuinfo() { nlohmann::json j = json::parse(kernel->ssCPU()); }
void Shell::queueschell() {}
void Shell::execute() { this->kernel->executeSystem(); }
void Shell::kill() { this->kernel->reboot(); }
void Shell::loop() {
  while (true) {

    char readline[256];
    printf("~$ ");
    std::cin.getline(readline, 256);
    if (std::strcmp(readline, "help") == 0) {
      help();
    } else if (std::strcmp(readline, "meminfo") == 0) {
      meminfo();
    } else if (std::strcmp(readline, "cpuinfo") == 0) {
      cpuinfo();
    } else if (std::strcmp(readline, "queueschell") == 0) {
      queueschell();
    } else if (std::strcmp(readline, "execute") == 0) {
      execute();
    } else if (std::strcmp(readline, "kill -9") == 0) {
      kill();
    } else {
      printf("bash: %s : Comando nao encontrado\n", readline);
    }
  }
}