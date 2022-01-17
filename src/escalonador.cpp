#include "escalonador.hpp"
#include "process.hpp"
#include <algorithm>
#include <chrono>
#include <iostream> //TODO: Retirar prints de debug
#include <thread>

Escalonador::Escalonador(enum schedulerPolicy policy) : policy(policy) {
  srand(policy);
}
enum schedulerPolicy Escalonador::getPolicy() const { return this->policy; }
void Escalonador::applyPolicy(std::list<Process> &listOfProcess) {
  printf("Applying policy:\n");
  std::vector<Process> arrayP{std::make_move_iterator(listOfProcess.begin()),
                              std::make_move_iterator(listOfProcess.end())};
  switch (this->policy) {
  case schedulerPolicy::FIFO:
    listOfProcess = this->FIFO(arrayP);
    break;
  }
}
// TODO: Consumir timestamp. Verificar se o uso do Quantum esta de forma correta
// TODO: Criar classe/funcao de Log para fazer output em um json
void Escalonador::makeCycle(std::list<Process> &listProcess) {
  printf("Making cycle\n");
  int seconds{};
  int timestamp{};
  for (Process &ps : listProcess) {
    if (!ps.isProcessTerminated()) {
      printf("Consumindo ");
      if (ps.getResourceConsumed() == resources::ram ||
          ps.getResourceConsumed() == resources::disk) {
        ps.changeState(states::bloqueado);
        seconds = rand() % 4 + 1;
        printf("%s stopping for %d seconds\n",
               (ps.getResourceConsumed() == resources::ram ? "ram" : "disk"),
               seconds);
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        ps.changeState(states::pronto);
        timestamp += seconds;
      } else {
        printf("cpu\n");
      }
      ps.changeState(states::execucao);
      seconds = (rand() % ps.getMaxQuantum()) + 1;
      if (ps.getQuantum() - seconds < 0) {
        seconds = ps.getQuantum();
      }
      timestamp += seconds;
      printf(
          "PID:%d\t Seconds:%d maxQuantum:%d quantumAtual:%d\t timestamp:%d\t  "
          "ciclosRestantes: %d\n ",
          ps.getPID(), seconds, ps.getMaxQuantum(), ps.getQuantum(), timestamp,
          ps.getCycles());
      std::this_thread::sleep_for(std::chrono::seconds(seconds));
      ps.changeState(states::pronto);
      ps.changeResourceConsumed((rand() % 3));
      ps.makeCycle(timestamp, seconds);
    }
  }
}

//  Scheduling Policy
// FIFO Nao faz nada eu acho (y)
std::list<Process> Escalonador::FIFO(std::vector<Process> &listOfProcess) {
  return std::list<Process>{std::make_move_iterator(listOfProcess.begin()),
                            std::make_move_iterator(listOfProcess.end())};
}