module full_adder(
    input logic a,
    input logic b,
    input logic carry_in,
    output logic sum,
    output logic carry_out
);


logic inter_sum;

assign inter_sum = a ^ b;
assign sum = inter_sum ^ carry_in;
assign carry_out = inter_sum & carry_in | a & b;


endmodule