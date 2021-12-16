#include "cpu.hpp"
#include "disk.hpp"
#include "ram.hpp"
class Shell{
     const CPU & cpu;
     const RAM &ram;
     const Disk & disk;
     Shell(const CPU &cpu,const RAM& ram, const Disk & disk);

};