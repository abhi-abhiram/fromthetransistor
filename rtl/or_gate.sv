module or_gate(
    input logic a,
    input logic b,
    output logic led
);


assign led = a | b;  

endmodule