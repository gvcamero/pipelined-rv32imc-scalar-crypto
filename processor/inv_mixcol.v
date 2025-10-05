//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.09.2025 23:44:41
// Design Name: 
// Module Name: inv_mixcol
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

module inv_mixcol(
    input [7:0] in_byte,
    output [31:0] partial_mix
);
    
    wire [7:0] gftimesB;
    gfmul GF_B(
        .in_byte(in_byte),
        .mltplr(4'hB),
        .out_byte(gftimesB)
    );
    
    wire [7:0] gftimesD;
    gfmul GF_D(
        .in_byte(in_byte),
        .mltplr(4'hD),
        .out_byte(gftimesD)
    );
    
    wire [7:0] gftimes9;
    gfmul GF_9(
        .in_byte(in_byte),
        .mltplr(4'h9),
        .out_byte(gftimes9)
    );
    
    wire [7:0] gftimesE;
    gfmul GF_E(
        .in_byte(in_byte),
        .mltplr(4'hE),
        .out_byte(gftimesE)
    );
    
    assign partial_mix = {gftimesB, gftimesD, gftimes9, gftimesE};
    
endmodule
