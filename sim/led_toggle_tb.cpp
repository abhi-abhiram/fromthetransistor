#include <Vled_toggle.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <cassert>
#include <iostream>

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    Vled_toggle led;

    Verilated::traceEverOn(true);

    VerilatedVcdC trace;
    led.trace(&trace, 5);
    trace.open("waveform.vcd");

    vluint64_t time = 0;

    int CLK = 100;

    int toggle_count = 0;
    int toggle = 0;
    int before, after = 0;
    for (int temp = 0; temp < CLK; temp++)
    {

        led.clk = 0;
        led.eval();
        before = led.led;
        trace.dump(time++);
        led.clk = 1;
        led.eval();
        after = led.led;
        trace.dump(time++);
        if (before != after)
        {
            toggle++;
        }
    }

    assert(toggle == 3);

    trace.close();
}