#include <Vregister.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <cassert>

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    Vregister reg;

    Verilated::traceEverOn(true);

    VerilatedVcdC trace;
    reg.trace(&trace, 5);
    trace.open("waveform.vcd");

    vluint64_t time = 0;

    reg.clk = 0;
    reg.reset = 0;
    reg.enable = 1;
    reg.d = 0x01;
    reg.eval();
    trace.dump(time++);

    reg.clk = 1;
    reg.eval();
    trace.dump(time++);
    assert(reg.q == 0x01);

    reg.clk = 0;
    reg.enable = 0;
    reg.eval();
    trace.dump(time++);
    assert(reg.q == 0x01);

    reg.reset = 1;
    reg.clk = 1;
    reg.eval();
    trace.dump(time++);
    assert(reg.q == 0x00);

    reg.clk = 0;
    reg.eval();
    trace.dump(time++);
    assert(reg.q == 0x00);

    trace.close();
}