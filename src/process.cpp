#include "process.hpp"
Process::Process(const int pid, const int cycles, const int maxQuantum,
                 enum resources initBound, enum priorities priority)
    : pid(pid), cycles(cycles), maxQuantum(maxQuantum),
      resourceConsumed(initBound), priority(priority), timestamp(0) {}
Process::Process(const int pid, const int cycles, const int maxQuantum,
                 enum resources initBound, enum priorities priority,
                 const int timestamp)
    : pid(pid), cycles(cycles), maxQuantum(maxQuantum),
      resourceConsumed(initBound), priority(priority), timestamp(timestamp) {}

void Process::changeState(enum ::states state) { this->state = state; }
void Process::changePriority(const enum ::priorities priority) {
  this->priority = priority;
}
void Process::changeResourceConsumed(enum resources rs) {
  this->resourceConsumed = rs;
}
void Process::changeResourceConsumed(int rs) {
  this->resourceConsumed = static_cast<enum resources>(rs);
}
void Process::makeCycle() {
  if (!this->isProcessTerminated())
    this->cycles--;
  if (this->cycles <= 0)
    this->state = states::finalizado;
}
enum resources Process::getResourceConsumed() const {
  return this->resourceConsumed;
}
int Process::getPID() const { return this->pid; }
int Process::getQuantum() const { return this->quantum; }
int Process::getMaxQuantum() const { return this->maxQuantum; }
int Process::getPriority() const { return this->priority; }
int Process::getTimestamp() const { return this->timestamp; }
int Process::getCycles() const { return this->cycles; }
bool Process::isProcessTerminated() const {
  return this->state == states::finalizado;
}