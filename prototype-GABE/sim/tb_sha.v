//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 21.09.2025 16:12:56
// Design Name: 
// Module Name: tb_sha
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module tb_sha();
    
    reg [`WORD_WIDTH-1:0] in_word;
    reg [1:0] sel;
    wire [`WORD_WIDTH-1:0] out_word;
    
    sha SHA(
        .in_word(in_word),
        .sel(sel),
        .out_word(out_word)
    );
    
    initial begin
        in_word = 32'hA12B6CD9;
        sel = 2'b00;
        #10
        sel = 2'b01;
        #10
        sel = 2'b10;
        #10
        sel = 2'b11;
        #10
        $finish;
    end
    
endmodule
