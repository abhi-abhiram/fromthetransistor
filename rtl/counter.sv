module counter #(
    parameter WIDTH = 8
)(
    input wire clk,
    input wire reset,
    input wire enable,
    output reg [WIDTH-1:0] count
);


always @(posedge clk) begin
    if (reset)
        count <= 0;
    else if(enable)
        count <= count + 1;
    
end

    
endmodule