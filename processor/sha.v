//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 20.09.2025 21:39:12
// Design Name: 
// Module Name: sha
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

module sha(
    input [`WORD_WIDTH-1:0] in_word,
    input [1:0] sel, // bits [21:20] of instruction
    output reg [`WORD_WIDTH-1:0] out_word
);

    // sig0
    wire [31:0] sig0 = 
        ((in_word >> 7) | (in_word << 25)) ^
        ((in_word >> 18) | (in_word << 14)) ^
        (in_word >> 3);
    
    // sig1
    wire [31:0] sig1 = 
        ((in_word >> 17) | (in_word << 15)) ^
        ((in_word >> 19) | (in_word << 13)) ^
        (in_word >> 10);
        
    // sum0
    wire [31:0] sum0 =
        ((in_word >> 2) | (in_word << 30)) ^
        ((in_word >> 13) | (in_word << 19)) ^
        ((in_word >> 22) | (in_word << 10));
        
    // sum1
    wire [31:0] sum1 =
        ((in_word >> 6) | (in_word << 26)) ^
        ((in_word >> 11) | (in_word << 21)) ^
        ((in_word >> 25) | (in_word << 7));
        
    always@(*) begin
        case(sel)
            2'b00: out_word = sum0;
            2'b01: out_word = sum1;
            2'b10: out_word = sig0;
            2'b11: out_word = sig1;
        endcase
    end
    
endmodule
