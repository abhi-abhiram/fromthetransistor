module d_flipflop (
    input logic d,
    input logic clk,
    input logic reset, 
    output reg q
);

always @(posedge clk) begin
    if (reset)
        q <= 0;
    else
        q <= d;
end
    
endmodule