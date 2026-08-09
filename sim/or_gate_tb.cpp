#include <Vor_gate.h>
#include <cassert>

int main()
{

    Vor_gate gate;

    gate.a = 0;
    gate.b = 0;
    gate.eval();
    assert(gate.led == 0);

    gate.a = 1;
    gate.b = 0;
    gate.eval();
    assert(gate.led == 1);

    gate.a = 0;
    gate.b = 1;
    gate.eval();
    assert(gate.led == 1);

    gate.a = 1;
    gate.b = 1;
    gate.eval();
    assert(gate.led == 1);

    return 0;
}