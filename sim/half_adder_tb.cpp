#include <Vhalf_adder.h>
#include <cassert>

int main()
{

    Vhalf_adder adder;

    adder.a = 0;
    adder.b = 0;
    adder.eval();
    assert(adder.sum == 0);
    assert(adder.carry == 0);

    adder.a = 1;
    adder.b = 0;
    adder.eval();
    assert(adder.sum == 1);
    assert(adder.carry == 0);

    adder.a = 0;
    adder.b = 1;
    adder.eval();
    assert(adder.sum == 1);
    assert(adder.carry == 0);

    adder.a = 1;
    adder.b = 1;
    adder.eval();
    assert(adder.sum == 0);
    assert(adder.carry == 1);

    return 0;
}