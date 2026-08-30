#include <Vshift_register.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <bitset>

// Note: Didn't added bidirectional test
int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    Vshift_register s_reg;

    std::bitset<8> x8(0b00001000);

    Verilated::traceEverOn(true);

    VerilatedVcdC trace;
    s_reg.trace(&trace, 5);
    trace.open("waveform.vcd");

    vluint64_t time = 0;

    // initialization
    s_reg.clk = 0;
    s_reg.serial_in = 0;
    s_reg.enable = 1;
    s_reg.reset = 0;
    s_reg.eval();
    trace.dump(time++);

    // stream bits from reg into s_reg
    for (int temp = 7; temp >= 0; temp--)
    {
        s_reg.serial_in = x8[temp];
        s_reg.eval();
        trace.dump(time++);

        s_reg.clk = 1;
        s_reg.eval();
        trace.dump(time++);

        s_reg.clk = 0;
        s_reg.eval();
        trace.dump(time++);
    }

    trace.close();
}
