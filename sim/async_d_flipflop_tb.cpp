#include <Vasync_d_flipflop.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    Vasync_d_flipflop dut;

    Verilated::traceEverOn(true);

    VerilatedVcdC trace;
    dut.trace(&trace, 5);
    trace.open("waveform.vcd");

    vluint64_t time = 0;

    dut.d = 0;
    dut.clk = 0;
    dut.eval();
    trace.dump(time++);

    dut.clk = 1;
    dut.eval();
    trace.dump(time++);

    dut.clk = 0;
    dut.d = 1;
    dut.eval();
    trace.dump(time++);

    dut.clk = 1;
    dut.eval();
    trace.dump(time++);

    dut.reset = 1;
    dut.clk = 0;
    dut.eval();
    trace.dump(time++);

    dut.clk = 0;
    dut.eval();
    trace.dump(time++);

    trace.close();
}
