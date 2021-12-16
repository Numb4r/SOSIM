#include "kernel.hpp"
#include "json.hpp"
Kernel::Kernel(const char* fileProcessPath){
    
    nlohmann::json j = Bootloader().boot(fileProcessPath);
    std::string  pc= j["processCount"];
    this->escalonador = Escalonador(atoi(pc.c_str()));
    for (auto&&i : j["process"]) {
        //escalonador.addProcessToList(i);
    }
}
Kernel::Kernel(){
    this->escalonador = Escalonador(1);
    printf("%d",escalonador.getProcessCountMax());
}
