#include "process.hpp"


Process::Process(const int pid):pid(pid),priority(medium),timestamp(0),quantum(0),state(criado){}
Process::Process(const int pid,const enum::priorities priority):pid(pid),priority(priority),timestamp(0),quantum(0),state(criado){}
void Process::changeState(enum ::states state){
    this->state = state;
}
void Process::changePriority(const enum ::priorities priority){
    this->priority = priority;
}
void Process::setQuantum(const int quantum){
    this->quantum = quantum;
}
void Process::addTimestamp(const int add){
    this->timestamp+= add;
}
int Process::getPID() const{
    return this->pid;
}
int Process::getPriority() const{
    return this->priority;
}
int Process::getTimestamp() const{
    return this->timestamp;
}