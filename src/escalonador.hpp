#pragma once
#include <list>
#include "process.hpp"



class Escalonador{
        int processCountMax;
        int pc;
        std::list<Process> listProcess;
    public:
        Escalonador();
        Escalonador(int processCountMax);
        int getProcessCountMax();
        int getPc();
        void addProcessToList();
       

};