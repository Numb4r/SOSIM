#pragma once
#include "bootloader.hpp"
#include "escalonador.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "disk.hpp"
#include "shell.hpp"
class Kernel{
        CPU cpu;
        RAM ram;
        Disk disk;
        Escalonador escalonador;
        Shell shell;
    public:
        Kernel(const char* fileProcessPath);

};