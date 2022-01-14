#include "escalonador.hpp"
#include "process.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
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
      int processo = processInfo["processo"];
      int ciclos = processInfo["ciclos"];
      int max_quantum = processInfo["max_quantum"];
      enum resources init_type = this->translateToEnum.at(processInfo["init_type"]);
      enum priorities prioridade = processInfo["prioridade"];
      int timestamp =  processInfo["timestamp"];
  this->listProcess.push_back(
    Process(
      processo,
      ciclos,
      max_quantum,
      init_type,
      prioridade,
      timestamp
      ));
}

static int lastPID = 0;
static int cyclesQuantum = 0;
void Escalonador::createProcess(const int cycles) {
  if (cycles % this->intervalCreation == 0) {
    if (listProcess.empty())
      return;
    Process ps = listProcess.front();
    listProcess.pop_front();
    ps.changeState(states::criado);
    queueProcess.push(ps);
    this->pc++;
  }
}

// TODO : TA BUGADO (Y)
void Escalonador::resetEscalonador(const int pc, const int it) {
  this->processCountMax = pc;
  this->pc = 0;
  this->intervalCreation = it;
  lastPID = 0;
  cyclesQuantum = 0;
  std::queue<Process> emptyQ;
  std::vector<Process> emptyV;
  std::list<Process> emptyL;
  std::swap(this->queueProcess, emptyQ);
  std::swap(this->listProcess, emptyL);
  std::swap(this->deadProcess, emptyV);
}

// TODO: REESCREVER POLITICA EM OUTRA CLASSE
// REALIZAR POLIMORFISMO PARA ISSO
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