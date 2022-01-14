#include "process.hpp"
Process::Process(const int pid,const int cycles,const int maxQuantum,enum resources initBound, enum priorities priority):
pid(pid),cycles(cycles),maxQuantum(maxQuantum),initBound(initBound),priority(priority),timestamp(0){}
Process::Process(const int pid,const int cycles,const int maxQuantum,enum resources initBound, enum priorities priority,const int timestamp):
pid(pid),cycles(cycles),maxQuantum(maxQuantum),initBound(initBound),priority(priority),timestamp(timestamp){}
    
void Process::changeState(enum ::states state) { this->state = state; }
void Process::changePriority(const enum ::priorities priority) {
  this->priority = priority;
}

void Process::makeCycle() { this->timestamp++; }

int Process::getPID() const { return this->pid; }
int Process::getQuantum() const { return this->quantum; }
int Process::getPriority() const { return this->priority; }
int Process::getTimestamp() const { return this->timestamp; }
int Process::getCycles() const { return this->cycles; }
bool Process::isTerminated() const { return timestamp >= cycles; }