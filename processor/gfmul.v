//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.09.2025 23:45:58
// Design Name: 
// Module Name: gfmul
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
`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module gfmul(
    input [7:0] in_byte,
    input [3:0] mltplr,
    output [7:0] out_byte
);

    wire [7:0] gf2;
    gfmul_2 GF2(
        .in_byte(in_byte),
        .out_byte(gf2)
    );
    
    wire [7:0] gf2_2;
    gfmul_2 GF2_2(
        .in_byte(gf2),
        .out_byte(gf2_2)
    );
    
    wire [7:0] gf2_2_2;
    gfmul_2 GF2_2_2(
        .in_byte(gf2_2),
        .out_byte(gf2_2_2)
    );
    
    assign out_byte = 
        (mltplr[0] ? in_byte : 8'h00) ^
        (mltplr[1] ? gf2 : 8'h00) ^
        (mltplr[2] ? gf2_2 : 8'h00) ^
        (mltplr[3] ? gf2_2_2 : 8'h00);
    
endmodule
