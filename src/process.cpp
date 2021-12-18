#include "process.hpp"

Process::Process(int cycles, std::queue<resources> queueResources)
    : cycles(cycles), queueResources(queueResources) {}
void Process::changeState(enum ::states state) { this->state = state; }
void Process::changePriority(const enum ::priorities priority) {
  this->priority = priority;
}

void Process::makeCycle() { this->timestamp++; }

void Process::setQuantum(const int quantum) { this->quantum = quantum; }

int Process::getPID() const { return this->pid; }
int Process::getQuantum() const { this->quantum; }
int Process::getPriority() const { return this->priority; }
int Process::getTimestamp() const { return this->timestamp; }
int Process::getCycles() const { this->cycles; }
bool Process::isTerminated() const { return timestamp >= cycles; }