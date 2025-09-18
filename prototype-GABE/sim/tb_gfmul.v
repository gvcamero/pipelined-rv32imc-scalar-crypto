//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.09.2025 13:50:08
// Design Name: 
// Module Name: tb_gfmul
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

module tb_gfmul();

    reg [7:0] in_byte;
    wire [7:0] out_byte;
    
    gfmul GFMUL(
        .in_byte(in_byte),
        .out_byte(out_byte)
    );
    
    initial begin
        in_byte = 8'hAC;
        #10
        in_byte = 8'h73;
        #10
        $finish;
    end

endmodule
