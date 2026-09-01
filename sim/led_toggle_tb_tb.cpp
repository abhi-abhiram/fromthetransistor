#include <iostream>
#include <cstdlib> // Required for std::system
#include <cstring>
#include <list>

int main(int argc, char **argv)
{

    const char *BASE = "verilator --cc --exe --build --trace ./rtl/led_toggle.sv ./rtl/counter.sv  ./sim/led_toggle_tb.cpp --top-module led_toggle";

    const char *CLK_FREQ_HZ_ARG = "-GCLK_FREQ_HZ", *HALF_PERIOD_MS_ARG = "-GHALF_PERIOD_MS";

    // no of test cases
    int testcs = 0;

    // default clk
    std::list<u_int32_t> clks = {50'000'000};

    for (int i = 1; i < argc; i++)
    {
        if (std::strcmp(*(argv + i), "test_cases") == 0)
        {
            testcs = std::stoi(argv[++i]);

            int j;
            for (j = i + 1; j < i + testcs + 1; j++)
            {
                clks.push_back(std::stoi(argv[j]));
            }
            i = j;
        }
    }

       for (int i = 0; i < testcs + 1; i++)
    {
    }

    return 0;
}
