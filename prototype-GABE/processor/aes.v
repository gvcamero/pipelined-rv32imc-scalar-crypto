//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 14.09.2025 20:45:09
// Design Name: 
// Module Name: aes
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

module aes(

    input [`WORD_WIDTH-1:0] op_a, // rs1
	input [`WORD_WIDTH-1:0] op_b, // rs2
	input [1:0] bs, // byte selector
	input is_mid, // check if middle round
	input mode, // select encrypt or decrypt (enc = 0, dec = 1)
	
	output [`WORD_WIDTH-1:0] res
	
);

    // select byte
    wire [7:0] in_byte = (bs == 2'b00) ? op_b[7:0] :
                         (bs == 2'b01) ? op_b[15:8] :
                         (bs == 2'b10) ? op_b[23:16] : op_b[31:24];

    // fsbox
    wire [7:0] fsbox_out; // fsbox output
    fsbox FSBOX(
        .in_byte(in_byte),
        .out_byte(fsbox_out)
    );
    
    // fwd mixcolumns
    wire [31:0] fwd_mixcol_out; // mixcol output
    fwd_mixcol FWD_MIXCOL(
        .in_byte(fsbox_out),
        .partial_mix(fwd_mixcol_out)
    );
    
    //

endmodule
