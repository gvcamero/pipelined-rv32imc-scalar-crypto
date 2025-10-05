//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.09.2025 23:38:01
// Design Name: 
// Module Name: gfmul_2
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

module gfmul_2(
    input [7:0] in_byte,
    output [7:0] out_byte
);

    /*wire [7:0] shifted = {in_byte[6:0], 1'b0};
    wire [7:0] shifted_xor = shifted ^ 8'h1B;
    
    assign out_byte = in_byte[7] ? shifted_xor : shifted;*/
    
    assign out_byte = (in_byte << 1) ^ (in_byte[7] ? 8'h1B : 8'h00);

endmodule
