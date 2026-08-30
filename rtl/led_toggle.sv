/*

clk is 50mhz. We want led to every 0.5 secs, which is equal to 2 cycles per second. 

No of raising edges in clk is 50 * 10^6. 
Count upto the 1/2 of clk's frequency, that is count == 1/2 * 50 * 10^6

That gives us counter width of log2(1/2 * 50 * 10^6) = 24.5754247591 ~ 25 bits 

[6, 11, 12, 13, 14, 16, 18, 19, 20, 21, 22, 24] = 25 million

*/

module led_toggle #(
    parameter int CLK_FREQ_HZ = 50_000_000,
    parameter int HALF_PERIOD_MS = 500
)(
    input  logic clk,
    output logic led = 0
);

localparam int COUNT_MAX =
    (CLK_FREQ_HZ / 1000) * HALF_PERIOD_MS;

localparam int WIDTH =
    $clog2(COUNT_MAX);

logic [WIDTH - 1:0] count;
logic toggle;

counter #(.WIDTH(WIDTH)) c1 (
    .clk(clk),
    .reset(toggle),
    .enable(1'b1),
    .count(count)
);

assign toggle = (count == WIDTH'(COUNT_MAX - 1));

always @(posedge clk) begin
    if (toggle)
        led <= !led;
end

endmodule