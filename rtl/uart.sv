module uart #(
    parameter int CLK_FREQ_HZ=50_000_000,
    parameter int BAUD=9600
)(
    input logic request = 0,
    input logic clk,
    input logic [7:0] data_in,
    output logic data_out,
    output logic ready
);

localparam int COUNT_MAX= CLK_FREQ_HZ/BAUD;
localparam int WIDTH = $clog2(COUNT_MAX);

logic [WIDTH - 1:0] baud_count;
logic [7:0] data_frame;
logic toggle;
logic start = 0;
logic [3:0] data_count;
logic data_bit = 0;
logic req_pre = 0;


counter #(.WIDTH(WIDTH)) c1 (
    .clk(clk),
    .reset(toggle),
    .enable(start),
    .count(baud_count)
);

assign toggle = (baud_count == WIDTH'(COUNT_MAX-1));

counter #(.WIDTH(4)) c2 (
    .clk(clk),
    .reset(!start),
    .enable(start & toggle),
    .count(data_count)
);

always @(posedge clk) begin
    if (!req_pre & request & !start) begin
        data_frame[7:0] <= data_in;
        start <= 1;
    end

    req_pre <= request;

    if (start & toggle) begin
        if(data_count == 0) 
            data_bit <= data_frame[0];
        if(data_count == 1)
            data_bit <= data_frame[1];
        else if(data_count == 2)
            data_bit <= data_frame[2];
        else if (data_count == 3) 
            data_bit <= data_frame[3];
        else if(data_count == 4)
            data_bit <= data_frame[4];
        if(data_count == 5)
            data_bit <= data_frame[5];
        else if(data_count == 6) 
            data_bit <= data_frame[6];
        else if (data_count == 7) 
            data_bit <= data_frame[7];
        else if(data_count == 8) 
            data_bit <= 1;
        else if(data_count == 9)  
            start <= 0;
            data_bit <= 0;
    end 
end


assign data_out = !start | data_bit;

assign ready = !start;


endmodule