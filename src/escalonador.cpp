#include "escalonador.hpp"
#include "process.hpp"
#include <algorithm>
#include <chrono>
#include <iostream> //TODO: Retirar prints de debug
#include <thread>
#include <random>

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
  case schedulerPolicy::LRU:
    listOfProcess = this->LRU(arrayP);
    break;
  case schedulerPolicy::MLQ:
    listOfProcess = this->MLQ(arrayP);
    break;
  case schedulerPolicy::PSLS:
    listOfProcess = this->PSLS(arrayP);
    break;
  }
  
}
// TODO: Consumir timestamp. Verificar se o uso do Quantum esta de forma correta
// TODO: Criar classe/funcao de Log para fazer output em um json
void Escalonador::makeCycle(std::list<Process> &listProcess, Log &log) {
  printf("Making cycle\n");
  int seconds{};
  int secondsIO{};
  int timestamp{};
  for (Process &ps : listProcess) {
    if (!ps.isProcessTerminated()) {
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
// Sort de acordo com o com menor numero de ciclos
std::list<Process> Escalonador::LRU(std::vector<Process> &listOfProcess){
  std::sort(listOfProcess.begin(),listOfProcess.end(),[](Process a,Process b){
     return  a.getCycles() < b.getCycles();
  });
  return std::list<Process>{listOfProcess.begin(),listOfProcess.end()};
}
//TODO: Implementar loteria
std::list<Process> Escalonador::MLQ(std::vector<Process> &listOfProcess){
  std::vector<std::vector<Process>> Ml ={{}, {}, {},{},{},{}};
  for(auto &&i:listOfProcess){
    Ml.at(i.getPriority()).push_back(i);
  }
  //Aplicando PSLS
  for(auto &&i:Ml){
   auto aux = this->PSLS(i);
   i.assign(aux.begin(),aux.end());
  }
  std::list<Process> mergedList;
  std::for_each(Ml.rbegin(),Ml.rend(),[&mergedList](std::vector<Process> vec) mutable {
    std::list<Process> list{vec.begin(),vec.end()};
      mergedList.splice(mergedList.end(),list,list.begin(),list.end());
  });
  return mergedList;
}
// Pseudo Lottery Scheduling 
// Apenas rearranja de acordo com um shuffle alimentado por um seed
// nao e exatamente o mesmo funcionamento do Lottery, mas acredito que isso basta para criar um
// ruido dentro de cada lista da politica MLQ
std::list<Process> Escalonador::PSLS(std::vector<Process> &listOfProcess){
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle (listOfProcess.begin(), listOfProcess.end(), std::default_random_engine(seed));
  return std::list<Process>{listOfProcess.begin(),listOfProcess.end()};
}