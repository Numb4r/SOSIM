#include "kernel.hpp"
#include "shell.hpp"

int main(const int argc,char** argv ){
    Kernel kernel(argv[1]);
    Shell shell(&kernel);
    shell.input();
   

    return 0;
}