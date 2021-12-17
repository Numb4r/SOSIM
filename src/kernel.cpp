#include "kernel.hpp"
#include "json.hpp"
Kernel::Kernel(const char* fileProcessPath):cpu(CPU()),ram(RAM(RAMMAX)),disk(Disk()){
    
    nlohmann::json j = Bootloader().boot(fileProcessPath);
    const int pc= j["processCount"];
    this->escalonador = Escalonador(pc);
    printf("%d",this->escalonador.getProcessCountMax());
    for (auto&&i : j["process"]) {
        //escalonador.addProcessToList(i);
    }
}

