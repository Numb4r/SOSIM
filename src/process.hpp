#pragma once
enum states{
    criado ,
    pronto,
    execucao,
    bloqueado,
    finalizado
};
enum priorities{
    realtime,
    high,
    medium,
    low,
    zero
};
class Process{
    int pid;
    int timestamp ;
    int quantum;
    enum::priorities priority;
    enum::states state;
    public:
        Process(const int pid);
        Process(const int pid,const enum::priorities priority);
        
        void changeState(enum::states state);
        void changePriority(const enum::priorities priority);
        void setQuantum(const int quantum);
        void addTimestamp(const int add);
        int getPID() const;
        int getPriority()const;
        int getTimestamp()const;
};
