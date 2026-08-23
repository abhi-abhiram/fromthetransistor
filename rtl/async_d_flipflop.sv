module async_d_flipflop(
    input logic d,
    input logic clk,
    input logic reset,
    output logic q
);


always @(posedge clk or posedge reset) begin
    if (reset)
        q <= 0;
    else
        q <= d;
end


endmodule

