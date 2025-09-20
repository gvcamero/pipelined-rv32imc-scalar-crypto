//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 18.09.2025 16:24:33
// Design Name: 
// Module Name: tb_fwd_mixcol
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

module tb_fwd_mixcol();

    reg [7:0] in_byte;
    wire [31:0] partial_mix;
    
    fwd_mixcol FWD_MIXCOL(
        .in_byte(in_byte),
        .partial_mix(partial_mix)
    );
    
    initial begin
        in_byte = 8'h3F;
        #10
        in_byte = 8'hB1;
        #10
        $finish;
    end

endmodule
