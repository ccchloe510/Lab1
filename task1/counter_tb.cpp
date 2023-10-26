#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;
    int halt_cycles = 0;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");
    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i=0; i<300; i++){

        // Check if the counter has reached 0x9
        if (top->count == 0x9) {
            halt_cycles = 3; // We will halt for 3 cycles
        }

        if (halt_cycles > 0) {
            top->en = 0; // Stop the counter
            halt_cycles--;
        } else {
            top->en = 1; // Resume the counter
        }

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);      //unit is in ps
            top->clk = !top->clk;
            top->eval();
        }
        top->rst = (i < 2) | (i == 15);

        if (Verilated::gotFinish())  exit(0);
    }
    tfp->close();
    exit(0);
}

  