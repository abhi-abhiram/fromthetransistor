module full_adder4 (
    input logic [3:0] a,
    input logic [3:0] b,
   output logic [3:0] sum,
   output logic carry 
);



wire c0;


full_adder adder0 (
    .a(a[0]),
    .b(b[0]),
    .carry_in(0),
    .sum(sum[0]),
    .carry_out(c0)
);

wire c1;

full_adder adder1 (
    .a(a[1]),
    .b(b[1]),
    .carry_in(c0),
    .sum(sum[1]),
    .carry_out(c1)
);

wire c2;

full_adder adder2 (
    .a(a[2]),
    .b(b[2]),
    .carry_in(c1),
    .sum(sum[2]),
    .carry_out(c2)
);

full_adder adder3 (
    .a(a[3]),
    .b(b[3]),
    .carry_in(c2),
    .sum(sum[3]),
    .carry_out(carry)
);

    
endmodule