#include <Vfull_adder4.h>
#include <cassert>

int main()

{

    Vfull_adder4 adder4;

    adder4.a = 0x3;
    adder4.b = 0x3;
    adder4.eval();
    assert(adder4.sum == 0x6);
    assert(adder4.carry == 0);

    return 0;
}