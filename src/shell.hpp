#pragma once
#include "kernel.hpp"
class Shell{
     const Kernel* kernel;
     void help();        //help: Demonstra quais comandos podem ser executados pelo shell desenvolvido;
     void meminfo();     //meminfo: detalha quantas unidades de memória so sistema possui, quantas estão livres e quantas estão alocadas e para quais processos.
     void cpuinfo();     //cpuinfo: detalha qual processo está em execução, qual o timestamp e quantum desse.
     void queueschell(); // queueschell: detalha quais processos estão sendo gerenciados pelo seu sistema, quais estão em estado de pronto, bloqueado, execução e/ou sendo criados e finalizados.
     void execute();     //execute: Executa a fila de processos definida conforme configuração prévia. 
     void kill();        //kill -9: Finaliza a execução do sistema operacional, voltando o mesmo para o estado inicial, em que os processos estariam ainda em faze de criação inicial.
     public:
          Shell(const Kernel* kernel);    
          void input();
};