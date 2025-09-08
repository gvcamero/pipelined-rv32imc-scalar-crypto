//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08.09.2025 16:47:12
// Design Name: 
// Module Name: alu_bitmanip
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

module alu_bitmanip(

    input [`WORD_WIDTH-1:0] op_a,
	input [`WORD_WIDTH-1:0] op_b,
	input [3:0] ALU_op,
	
	output reg [`WORD_WIDTH-1:0] res
	
    );
    
    wire signed [`WORD_WIDTH-1:0] signed_a = op_a;
	wire signed [`WORD_WIDTH-1:0] signed_b = op_b;
	
	always@(*) begin
		case(ALU_op)
		    // base ISA
			`ALU_ADD: res = op_a + op_b;
			`ALU_SUB: res = op_a - op_b;
			`ALU_AND: res = op_a & op_b;
			`ALU_OR: res = op_a | op_b;
			`ALU_XOR: res = op_a ^ op_b;
			`ALU_SLT: res = signed_a < signed_b;
			`ALU_SLTU: res = op_a < op_b;
			`ALU_SLL: res = op_a << op_b[4:0];
			`ALU_SRL: res = op_a >> op_b[4:0];
			`ALU_SRA: res = signed_a >>> signed_b[4:0];
            // M extension
			/*`ALU_MUL: res = mul_res_reg;
			`ALU_MULHU: res = mul_res_reg;
			`ALU_MULH: res = mul_res_reg;
			`ALU_MULHSU: res = mul_res_reg;*/
            // division unimplemented
            
            //other ops 
			default: res = 32'h0;
		endcase
	end
    
endmodule
