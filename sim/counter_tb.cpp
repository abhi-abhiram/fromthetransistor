#include <Vcounter.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <cassert>

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    Vcounter counter;

    Verilated::traceEverOn(true);

    VerilatedVcdC trace;
    counter.trace(&trace, 5);
    trace.open("waveform.vcd");

    vluint64_t time = 0;

    counter.clk = 0;
    counter.reset = 0;
    counter.enable = 1;
    counter.eval();
    trace.dump(time++);

    for (int count = 1; count <= 2 * 255 + 1; count++)
    {
        if (counter.clk == 0)
        {
            counter.clk = 1;
        }
        else
        {
            counter.clk = 0;
        }
        counter.eval();
        trace.dump(time++);
    }

    // assert(counter.count == 5);

    trace.close();
}