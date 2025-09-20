//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 20.09.2025 14:53:35
// Design Name: 
// Module Name: tb_aes
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

module tb_aes();
    
    reg [`WORD_WIDTH-1:0] op_a;
    reg [`WORD_WIDTH-1:0] op_b;
    reg [1:0] bs;
    reg is_mid;
    reg mode;
    
    wire [`WORD_WIDTH-1:0] res;
    
    aes AES(
        .op_a(op_a),
        .op_b(op_b),
        .bs(bs),
        .is_mid(is_mid),
        .mode(mode),
        .res(res)
    );
    
    initial begin
        op_a = 32'h00000000;
        op_b = 32'hA8070564;
        bs = 0;
        is_mid = 1;
        mode = 1;
        #10
        op_a = res;
        bs = 1;
        #10
        op_a = res;
        bs = 2;
        #10
        op_a = res;
        bs = 3;
        #10
        $finish;
    end
    
endmodule
