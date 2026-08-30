module shift_register #(
    parameter WIDTH = 8
)(
    input logic clk,
    input logic reset,
    input logic enable,
    input logic serial_in,
    input logic dir,
    output reg [WIDTH-1:0] q
);

always @(posedge clk) begin
    if (reset)
        q <= 0;
    else if (enable)
        if (dir)
            q <= {q[6:0],serial_in};
        else 
            q <= {serial_in, q[1:6]}
end

endmodule