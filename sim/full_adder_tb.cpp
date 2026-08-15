#include <Vfull_adder.h>
#include <cassert>

// a b c_in sum c_out
// 0 0  0    0    0
// 1 0  1    0    1
// 1 1  1    1    1
// 0 1  0    1    0

int main()
{

    Vfull_adder adder;

    adder.a = 0;
    adder.b = 0;
    adder.carry_in = 0;
    adder.eval();
    assert(adder.sum == 0);
    assert(adder.carry_out == 0);

    adder.a = 1;
    adder.b = 0;
    adder.carry_in = 1;
    adder.eval();
    assert(adder.sum == 0);
    assert(adder.carry_out == 1);

    adder.a = 1;
    adder.b = 1;
    adder.carry_in = 1;
    adder.eval();
    assert(adder.sum == 1);
    assert(adder.carry_out == 1);

    adder.a = 0;
    adder.b = 1;
    adder.carry_in = 0;
    adder.eval();
    assert(adder.sum == 1);
    assert(adder.carry_out == 0);

    return 0;
}