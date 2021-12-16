#include "escalonador.hpp"
Escalonador::Escalonador(int processCountMax):processCountMax(processCountMax),pc(0){

}
int Escalonador::getProcessCountMax(){
    return this->processCountMax;
}
int Escalonador::getPc(){return pc;}
