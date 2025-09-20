//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 20.09.2025 11:00:10
// Design Name: 
// Module Name: tb_inv_mixcol
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

module tb_inv_mixcol();
    
    reg [7:0] in_byte;
    wire [31:0] partial_mix;
    
    inv_mixcol INV_MIXCOL(
        .in_byte(in_byte),
        .partial_mix(partial_mix)
    );
    
    initial begin
        in_byte = 8'h56;
        #10
        in_byte = 8'h9D;
        #10
        $finish;
    end
    
endmodule
