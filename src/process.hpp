#pragma once
#include <queue>
enum states { criado, pronto, execucao, bloqueado, finalizado, esperando };
enum priorities { zero, low, medium, high, realtime };
enum resources { cpu, ram, disk };
class Process {
  int pid;
  int timestamp;
  int quantum;
  int cycles;
  int maxQuantum;
  resources resourceConsumed;
  enum ::priorities priority;
  enum ::states state;

public:
  Process(const int pid, const int cycles, const int maxQuantum,
          enum resources initBound, enum priorities priority);
  Process(const int pid, const int cycles, const int maxQuantum,
          enum resources initBound, enum priorities priority,
          const int timestamp);
  Process() = default;

  void changeState(enum ::states state);
  void changePriority(const enum ::priorities priority);
  void makeCycle(const int timestamp, const int quantumReceive);

  int getPID() const;
  enum resources getResourceConsumed() const;
  void changeResourceConsumed(enum resources rs);
  void changeResourceConsumed(int rs);
  enum priorities getPriority() const;
  int getTimestamp() const;
  int getQuantum() const;
  int getCycles() const;
  int getMaxQuantum() const;
  enum states getState() const;
  bool isProcessTerminated() const;
  void decreasePriority();
};
