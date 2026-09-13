module uart #(
    parameter int CLK_FREQ_HZ=50_000_000,
    parameter int BAUD=9600
)(
    input logic clk,
    input  logic data_in,
    output logic [7:0] data_out,
    output logic ready
);

localparam int COUNT_MAX= CLK_FREQ_HZ/BAUD;
localparam int WIDTH = $clog2(COUNT_MAX);

logic [WIDTH - 1:0] baud_count;
logic [7:0] data_frame;
logic idle = 1;
logic toggle;
logic start = 0;
logic [3:0] data_count;


counter #(.WIDTH(WIDTH)) c1 (
    .clk(clk),
    .reset(toggle),
    .enable(start),
    .count(baud_count)
);

assign toggle = (baud_count == WIDTH'(COUNT_MAX - 1));
assign center = (baud_count == WIDTH'((COUNT_MAX/2) - 1));

counter #(.WIDTH(4)) c2 (
    .clk(clk),
    .reset(!start),
    .enable(start & center),
    .count(data_count)
);

always @(posedge clk) begin
    if (idle & !start & !data_in) begin
        start <= 1;
        idle  <= 0;
    end

    if(ready) begin 
       ready <= 0; 
    end


    if (start & center) begin
        if(data_count == 1)
            data_frame[0] <= data_in;
        else if(data_count == 2)
            data_frame[1] <= data_in;
        else if (data_count == 3) 
            data_frame[2] <= data_in;
        else if(data_count == 4)
            data_frame[3] <= data_in;
        else if(data_count == 5)
            data_frame[4] <= data_in;
        else if(data_count == 6) 
            data_frame[5] <= data_in;
        else if (data_count == 7) 
            data_frame[6] <= data_in;
        else if (data_count == 8)
            data_frame[7] <= data_in;
    end 

    if (start & toggle & data_count == 9) begin
        start <= 0;
        ready <= 1;
        idle <= 1;
    end
end


assign data_out = data_frame;

endmodule