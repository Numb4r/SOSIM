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
  printf("Applying policy: ");
  std::vector<Process> arrayP{std::make_move_iterator(listOfProcess.begin()),
                              std::make_move_iterator(listOfProcess.end())};
  for (int i = 0; i < arrayP.size(); i++) {
    if (arrayP.at(i).isProcessTerminated()) {
      // Escrever logica para pegar o log aqui quando o processo finalizar?
      //  Move ele para um lista de terminatedProcess?
      printf("erasing %d", arrayP.at(i).getPID());
      arrayP.erase(arrayP.begin() + i);
    }
  }

  switch (this->policy) {
  case schedulerPolicy::FIFO:
    listOfProcess = this->FIFO(arrayP);
    break;
  }
}
// TODO: Consumir timestamp. Verificar se o uso do Quantum esta de forma correta
// TODO: Criar classe/funcao de Log para fazer output em um json
void Escalonador::makeCycle(std::list<Process> &listProcess) {
  printf("Making cycle %d\n", listProcess.empty());
  int seconds{};
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
      } else {
        printf("cpu\n");
      }
      ps.changeState(states::execucao);
      seconds = (rand() % ps.getMaxQuantum()) + 1;
      printf("PID:%d\t Seconds:%d\t ciclosRestantes: %d\n ", ps.getPID(),
             seconds, ps.getCycles());
      std::this_thread::sleep_for(std::chrono::seconds(seconds));
      ps.changeState(states::pronto);
      ps.changeResourceConsumed((rand() % 3));
      ps.makeCycle();
    }
  }
}

//  Scheduling Policy
// FIFO Nao faz nada eu acho (y)
std::list<Process> Escalonador::FIFO(std::vector<Process> &listOfProcess) {
  return std::list<Process>{std::make_move_iterator(listOfProcess.begin()),
                            std::make_move_iterator(listOfProcess.end())};
}