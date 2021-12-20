#pragma once
#include <queue>
enum states { criado, pronto, execucao, bloqueado, finalizado };
enum priorities { realtime, high, medium, low, zero };
enum resources { cpu, ram, disk };
class Process {
  int pid;
  int timestamp;
  int quantum;
  int cycles;
  std::queue<resources> queueResources;
  enum ::priorities priority;
  enum ::states state;

public:
  Process(int cycles, std::queue<resources> queueResources);
  Process() = default;

  void setPID(const int pid);
  void changeState(enum ::states state);
  void changePriority(const enum ::priorities priority);
  void setQuantum(const int quantum);
  void makeCycle();

  int getPID() const;
  int getPriority() const;
  int getTimestamp() const;
  int getQuantum() const;
  int getCycles() const;
  bool isTerminated() const;
};
