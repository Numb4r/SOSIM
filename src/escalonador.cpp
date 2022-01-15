#include "escalonador.hpp"
#include "process.hpp"
#include <algorithm>

Escalonador::Escalonador(enum schedulerPolicy policy) : policy(policy) {
  srand(policy);
}
enum schedulerPolicy Escalonador::getPolicy() const { return this->policy; }
void Escalonador::applyPolicy(std::list<Process> &listOfProcess) {
  std::vector<Process> arrayP{std::make_move_iterator(listOfProcess.begin()),
                              std::make_move_iterator(listOfProcess.end())};
  switch (this->policy) {
  case schedulerPolicy::FIFO:
    listOfProcess = this->FIFO(arrayP);
    break;
  }
}
void Escalonador::makeCycle(std::list<Process> &listProcess) {

  // for (Process &ps : listProcess) {
  //   std::cout << ps.getPID() << "\n";
  // }
}

//  Scheduling Policy
// FIFO Nao faz nada eu acho (y)
std::list<Process> Escalonador::FIFO(std::vector<Process> &listOfProcess) {
  // std::cout << "List:\n";
  return std::list<Process>{std::make_move_iterator(listOfProcess.begin()),
                            std::make_move_iterator(listOfProcess.end())};
}