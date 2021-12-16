#include "../core/hardware/cpu/cpu.hpp"
#include "../core/hardware/disk/disk.hpp"
#include "../core/hardware/memory/ram.hpp"
class Shell{
     const CPU & cpu;
     const RAM &ram;
     const Disk & disk;
     Shell(const CPU &cpu,const RAM& ram, const Disk & disk);
     
};