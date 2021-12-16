#pragma once
#include "bootloader.hpp"
#include "escalonador.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "disk.hpp"

class Kernel{
        CPU cpu;
        RAM ram;
        Disk disk;
        Escalonador escalonador;
    public:
        Kernel(const char* fileProcessPath);
        Kernel();

};