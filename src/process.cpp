#include "process.hpp"
Process::Process(const int pid, const int cycles, const int maxQuantum,
                 enum resources initBound, enum priorities priority)
    : pid(pid), cycles(cycles), maxQuantum(maxQuantum), quantum(maxQuantum),
      resourceConsumed(initBound), priority(priority), timestamp(0),
      state(states::criado) {}
Process::Process(const int pid, const int cycles, const int maxQuantum,
                 enum resources initBound, enum priorities priority,
                 const int timestamp)
    : pid(pid), cycles(cycles), maxQuantum(maxQuantum), quantum(maxQuantum),
      resourceConsumed(initBound), priority(priority), timestamp(timestamp),
      state(states::criado) {}

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
void Process::makeCycle(const int timestamp, const int quantumReceive) {
  if (!this->isProcessTerminated()) {
    this->quantum -= quantumReceive;
    this->timestamp += timestamp;
    this->isCycled = false;
    if (this->quantum == 0) {
      this->isCycled = true;
      this->cycles--;
      quantum = maxQuantum;
      this->decreasePriority();
    }
  }

  if (this->cycles <= 0)
    this->state = states::finalizado;
}
enum resources Process::getResourceConsumed() const {
  return this->resourceConsumed;
}
enum states Process::getState() const { return this->state; }
int Process::getPID() const { return this->pid; }
int Process::getQuantum() const { return this->quantum; }
int Process::getMaxQuantum() const { return this->maxQuantum; }
enum priorities Process::getPriority() const { return this->priority; }
int Process::getTimestamp() const { return this->timestamp; }
int Process::getCycles() const { return this->cycles; }
bool Process::isProcessTerminated() const {
  return this->state == states::finalizado;
}
void Process::decreasePriority() {
  this->priority =
      static_cast<priorities>((this->priority > 0 ? this->priority - 1 : 0));
}