
#include <list>
struct process{
    int pid;
    int timestamp ;
    int quantum;
    process(int pid,int timestamp): pid(pid),timestamp(timestamp),quantum(0){}
};


class Escalonador{
        int processCountMax;
        int pc;
        std::list<process> listProcess;
    public:
        Escalonador();
        Escalonador(int processCountMax);
        int getProcessCountMax();
        int getPc();
        void addProcessToList();
       

};