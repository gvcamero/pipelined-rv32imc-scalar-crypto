//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.09.2025 23:37:06
// Design Name: 
// Module Name: fwd_mixcol
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

module fwd_mixcol(
    input [7:0] in_byte,
    output [31:0] partial_mix
);

    wire [7:0] gftimes2; // GF(2^8) times 2 output
    gfmul_2 GFMUL_2(
        .in_byte(in_byte),
        .out_byte(gftimes2)
    );
    
    wire [7:0] gftimes3 = gftimes2 ^ in_byte; // GF(2^8) times 3 output
    
    assign partial_mix = {gftimes3, {2{in_byte}}, gftimes2};

endmodule