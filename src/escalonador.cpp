#include "escalonador.hpp"
#include "process.hpp"
#include <algorithm>
#include <chrono>
#include <iostream> //TODO: Retirar prints de debug
#include <random>
#include <thread>
#include <vector>

Escalonador::Escalonador(enum schedulerPolicy policy) : policy(policy) {
  srand(4);
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
  case schedulerPolicy::LRU:
    listOfProcess = this->LRU(arrayP);
    break;
  case schedulerPolicy::MLQ:
    listOfProcess = this->MLQ(arrayP);
    break;
  }
}
void Escalonador::makeCycle(std::list<Process> &listProcess, Log &log) {
  printf("Making cycle\n");
  int seconds{};
  int secondsIO{};
  int timestamp{};
  for (Process &ps : listProcess) {
    if (!ps.isProcessTerminated() && ps.getState() == states::pronto) {
      printf("Consumindo ");
      secondsIO = 0;
      if (ps.getResourceConsumed() == resources::ram ||
          ps.getResourceConsumed() == resources::disk) {
        ps.changeState(states::bloqueado);
        secondsIO = rand() % 4 + 1;
        printf("%s stopping for %d seconds\n",
               (ps.getResourceConsumed() == resources::ram ? "ram" : "disk"),
               secondsIO);
        std::this_thread::sleep_for(std::chrono::seconds(secondsIO));
        ps.changeState(states::pronto);
        timestamp += secondsIO;
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
      log.addLog(ps.getPID(),
                 {timestamp, (seconds + secondsIO), ps.getCycles(), seconds,
                  ps.getResourceConsumed(), ps.getPriority()});
      if (ps.isProcessTerminated()) {
        ps.changeResourceConsumed((rand() % 3));
      }
      ps.makeCycle(timestamp, seconds);
    }
  }
}
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------- Scheduling Policy
// ----------------------------------------------
// ---------------------------------------------------------------------------------------------------------------

void changeCreatetoReady(std::vector<Process> &listOfProcess) {
  for (auto &&i : listOfProcess)
    if (i.getState() == states::criado)
      i.changeState(states::pronto);
}
// FIFO Nao faz nada eu acho (y)
std::list<Process> Escalonador::FIFO(std::vector<Process> &listOfProcess) {
  changeCreatetoReady(listOfProcess);
  return std::list<Process>{std::make_move_iterator(listOfProcess.begin()),
                            std::make_move_iterator(listOfProcess.end())};
}
// Sort de acordo com o com menor numero de ciclos
std::list<Process> Escalonador::LRU(std::vector<Process> &listOfProcess) {
  changeCreatetoReady(listOfProcess);
  std::sort(listOfProcess.begin(), listOfProcess.end(),
            [](Process a, Process b) { return a.getCycles() < b.getCycles(); });
  return std::list<Process>{listOfProcess.begin(), listOfProcess.end()};
}
// funcoes auxiliares a loteria
static std::vector<int> tickets;
void redistributeTickets(std::vector<Process> &listOfProcess) {
  // Cicla por todos os processos procurando por aqueles com estado pronto e que
  // completaram um ciclo. Se tiver com estado pronto, e removido todas
  // os tickets ja dados para eles.
  for (auto &&i : listOfProcess) {
    if (i.getState() == states::pronto && i.isAlreadyCycled()) {
      (void)std::remove(tickets.begin(), tickets.end(), i.getPID());
    } else {
      // calculo para redistribuicao de tickets
      int amountTicketsReceive =
          (4 - i.getPriority()) * (5 * i.getPriority() + 1);
      for (int j = 0; j < amountTicketsReceive; j++)
        tickets.push_back(i.getPID());
    }
  }
}
std::list<Process> applyLottery(std::vector<Process> &listOfProcess) {
  // Loteria aplicada na ultima fila do escalonador MLQ
  int pidSelected =
      tickets.at(rand() % tickets.size()); // pid selecionado para execucao
  for (auto &&i : listOfProcess) {
    if (i.getPID() != pidSelected) {
      i.changeState(states::esperando);
    } else {
      i.changeState(states::pronto);
    }
  }
  redistributeTickets(listOfProcess);
  return std::list<Process>{listOfProcess.begin(), listOfProcess.end()};
}
std::list<Process> Escalonador::MLQ(std::vector<Process> &listOfProcess) {
  static int indexOfPriority = 4;
  if (indexOfPriority > 0) {
    // Separacao por prioridade
    std::vector<std::vector<Process>> Ml = {{}, {}, {}, {}, {}, {}};
    for (auto &&i : listOfProcess) {
      Ml.at(i.getPriority()).push_back(i);
    }
    // Checagem se a fila da prioridade atual nao esta vazia, se estiver descer
    // a prioridade
    while (Ml.at(indexOfPriority).empty() && indexOfPriority >= 0) {
      indexOfPriority--;
    }
    // Primeira vez que a prioridade cai para 0. Caso nao fizesse esse call, a
    // primeira vez que a prioridade descesse pra 0 a fila iria rodar em FIFO
    if (indexOfPriority == 0)
      return applyLottery(listOfProcess);
    // Preparar os processos da prioridade atual para execucao. Todos os
    // processos de uma prioridade abaixo tem seu estado trocado para evitar
    // execucoes de processos que sofreram rebaixamento
    for (auto &&i : Ml.at(indexOfPriority)) {
      i.changeState(states::pronto);
    }
    for (auto &&i : Ml.at(indexOfPriority - 1)) {
      i.changeState(states::esperando);
    }

    // redistribuicao de tickets e juncao das filas
    redistributeTickets(listOfProcess);
    std::list<Process> mergedList;
    std::for_each(Ml.rbegin(), Ml.rend(),
                  [&mergedList](std::vector<Process> vec) mutable {
                    std::list<Process> list{vec.begin(), vec.end()};
                    mergedList.splice(mergedList.end(), list, list.begin(),
                                      list.end());
                  });
    return mergedList;

  } else {
    return applyLottery(listOfProcess);
  }
}