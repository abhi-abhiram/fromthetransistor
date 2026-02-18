#include "Vorgate.h"
#include "verilated.h"
#include <iostream>


int main() {
    Verilated::traceEverOn(false);

    Vorgate* top = new Vorgate;


    unsigned char* output = &top->y;

    top->a = 1;
    top->b = 1;
    top->eval();

    std::cout << "Result = " << (int)*output << "\n";

    top->a = 1;
    top->b = 0;
    top->eval();

    std::cout << "Result = " << (int)*output << "\n";

    top->a = 0;
    top->b = 0;
    top->eval();

    std::cout << "Result =" << (int)*output << "\n";

    delete top;
    return 0;
}
