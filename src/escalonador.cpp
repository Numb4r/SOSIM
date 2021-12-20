#include "escalonador.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
Escalonador::Escalonador(int processCountMax, const int intervalCreation)
    : processCountMax(processCountMax), intervalCreation(intervalCreation),
      pc(0) {
  srand(time(NULL));
}
Escalonador::Escalonador() {}
bool Escalonador::isQueueEmpty() { return this->queueProcess.empty(); }
bool Escalonador::isListEmpty() { return this->listProcess.empty(); }
int Escalonador::getProcessCountMax() { return this->processCountMax; }
int Escalonador::getPc() { return pc; }
void Escalonador::addProcessToList(nlohmann::basic_json<> processInfo) {
  std::queue<resources> resources;
  int cycles = processInfo["cycles"];
  for (int i = 0; i < cycles; i++) {
    std::string rs = processInfo["queueResources"][i];
    if (strcmp(rs.c_str(), "cpu") == 0)
      resources.push(resources::cpu);
    else if (strcmp(rs.c_str(), "ram") == 0)
      resources.push(resources::ram);
    else if (strcmp(rs.c_str(), "disk") == 0)
      resources.push(resources::disk);
  }
  this->listProcess.push_back(Process(cycles, resources));
}

static int lastPID = 0;
static int cyclesQuantum = 0;
void Escalonador::createProcess(const int cycles) {
  if (cycles % this->intervalCreation == 0) {
    if (listProcess.empty())
      return;
    Process ps = listProcess.front();
    listProcess.pop_front();
    ps.setPID(lastPID);
    lastPID++;
    ps.setQuantum(QUANTUMMAX);
    ps.changeState(states::criado);
    queueProcess.push(ps);

    this->pc++;
  }
}
Process *Escalonador::getNextProcess(int cycles) {
  // TODO: REESCREVER A LOGICA DO ESCALONADOR MUITO CONFUSA
  if (!this->queueProcess.empty() &&
      this->queueProcess.front().isTerminated()) {
    cyclesQuantum = cycles;
    this->queueProcess.front().changeState(states::finalizado);
    this->deadProcess.push_back(this->queueProcess.front());
    this->queueProcess.pop();
    if (this->isQueueEmpty())
      return nullptr;
    else
      return &this->queueProcess.front();

  } else if (!this->queueProcess.empty() &&
             (cycles - cyclesQuantum <
                  this->queueProcess.front().getQuantum() ||
              cycles == 0)) {
    this->queueProcess.front().changeState(states::execucao);
    return &this->queueProcess.front();
  } else {

    cyclesQuantum = cycles;

    this->queueProcess.push(this->queueProcess.front());

    this->queueProcess.pop();
    if (this->isQueueEmpty())
      return nullptr;
    else
      return &this->queueProcess.front();
  }
  return nullptr;
}