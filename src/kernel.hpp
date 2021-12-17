#pragma once
#define NUMCORES 1
#define RAMMAX 5
#define DISKSPACE 10
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

};