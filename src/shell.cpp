#include "shell.hpp"
#include <stdio.h>
#include <functional>
#include <iostream>
#include <string>
Shell::Shell(const Kernel* kernel ): kernel(kernel){}

void Shell::help(){
    printf("help:\tDemonstra quais comandos podem ser executados pelo shell desenvolvido\n");
    printf("meminfo:\tDetalha quantas unidades de memória so sistema possui, quantas estão livres e quantas estão alocadas e para quais processos.\n");
    printf("cpuinfo:\tDetalha qual processo está em execução, qual o timestamp e quantum desse.\n");
    printf("queueschell:\tDetalha quais processos estão sendo gerenciados pelo seu sistema, quais estão em estado de pronto, bloqueado, execução e/ou sendo criados e finalizados.\n");
    printf("execute:\tExecuta a fila de processos definida conforme configuração prévia.\n");
    printf("kill -9:\tFinaliza a execução do sistema operacional, voltando o mesmo para o estado inicial, em que os processos estariam ainda em faze de criação inicial.\n");
    
}
void Shell::meminfo(){}
void Shell::cpuinfo(){}
void Shell::queueschell(){}
void Shell::execute(){}
void Shell::kill(){}
void Shell::input(){
    while (true) {
    
        std::string readline;
        printf("~$ ");
        std::cin>>readline;
        if(std::strcmp(readline.c_str(),"help")==0){
            help();
        }else
        if(std::strcmp(readline.c_str(),"meminfo")==0){
            meminfo();
        }else
        if(std::strcmp(readline.c_str(),"cpuinfo")==0){
            cpuinfo();
        }else if(std::strcmp(readline.c_str(),"queueschell")==0){
            queueschell();
        }else if(std::strcmp(readline.c_str(),"execute")==0){
            execute();
        }else
        if(std::strcmp(readline.c_str(),"kill -9")==0){
            kill();
        }else{
            printf("bash: %s : Comando nao encontrado\n",readline.c_str());
        }
    }

    
}