#include <Vmux.h>
#include <cassert>

// a = 0x12, b = 0xAB, sel = 0 → y = 0x12
// a = 0x12, b = 0xAB, sel = 1 → y = 0xAB
// a = 0xFF, b = 0x00, sel = 0 → y = 0xFF
// a = 0xFF, b = 0x00, sel = 1 → y = 0x00

int main()
{

    Vmux mux;

    mux.a = 0x12;
    mux.b = 0xAB;
    mux.sel = 0;
    mux.eval();
    assert(mux.y == 0x12);

    mux.sel = 1;
    mux.eval();
    assert(mux.y == 0xAB);

    mux.a = 0xFF;
    mux.b = 0x00;
    mux.sel = 0;
    mux.eval();
    assert(mux.y == 0xFF);

    mux.sel = 1;
    mux.eval();
    assert(mux.y == 0x00);

    return 0;
}