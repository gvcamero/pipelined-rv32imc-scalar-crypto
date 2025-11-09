//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// alu.v -- ALU for the RV32IMC processor
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: Microlab 198 Single-cycle RISC-V Group (2SAY1819)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: alu.v
// Description:
//
// Revisions:
// Revision 0.01 - File Created
// Revision 1.00 - File modified by Pipelined RISC-V Group (2SAY1920)
// Additional Comments:
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module alu(
	input CLK,
	input nrst,
	input load_hazard,

	input [`WORD_WIDTH-1:0] op_a,
	input [`WORD_WIDTH-1:0] op_b,
	input [4:0] ALU_op,

	output reg [`WORD_WIDTH-1:0] res,
	output mul_stall,
	output z,
	output less,
	output signed_less
);

	wire signed [`WORD_WIDTH-1:0] signed_a = op_a;
	wire signed [`WORD_WIDTH-1:0] signed_b = op_b;
	wire [2*`WORD_WIDTH-1:0]  ext_a = {{`WORD_WIDTH{((ALU_op == `ALU_MULH) || (ALU_op == `ALU_MULHSU)) && op_a[`WORD_WIDTH-1]}}, op_a};
	wire [2*`WORD_WIDTH-1:0]  ext_b = {{`WORD_WIDTH{(ALU_op == `ALU_MULH) && op_b[`WORD_WIDTH-1]}}, op_b};
        
    `ifdef FEATURE_MULT   
        wire [63:0] mul_res = ext_a * ext_b;
    `else 
        wire [63:0] mul_res = 0;
    `endif
         
	assign z	=	op_a == op_b;
	assign less =	op_a < op_b;
	assign signed_less	=	signed_a < signed_b;
    
    wire [63:0] clmul_res = (op_b[0] ? {{32{1'b0}}, op_a} : {64{1'b0}}) ^
	                        (op_b[1] ? {{31{1'b0}}, op_a, 1'b0} : {64{1'b0}}) ^
	                        (op_b[2] ? {{30{1'b0}}, op_a, {2{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[3] ? {{29{1'b0}}, op_a, {3{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[4] ? {{28{1'b0}}, op_a, {4{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[5] ? {{27{1'b0}}, op_a, {5{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[6] ? {{26{1'b0}}, op_a, {6{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[7] ? {{25{1'b0}}, op_a, {7{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[8] ? {{24{1'b0}}, op_a, {8{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[9] ? {{23{1'b0}}, op_a, {9{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[10] ? {{22{1'b0}}, op_a, {10{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[11] ? {{21{1'b0}}, op_a, {11{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[12] ? {{20{1'b0}}, op_a, {12{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[13] ? {{19{1'b0}}, op_a, {13{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[14] ? {{18{1'b0}}, op_a, {14{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[15] ? {{17{1'b0}}, op_a, {15{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[16] ? {{16{1'b0}}, op_a, {16{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[17] ? {{15{1'b0}}, op_a, {17{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[18] ? {{14{1'b0}}, op_a, {18{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[19] ? {{13{1'b0}}, op_a, {19{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[20] ? {{12{1'b0}}, op_a, {20{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[21] ? {{11{1'b0}}, op_a, {21{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[22] ? {{10{1'b0}}, op_a, {22{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[23] ? {{9{1'b0}}, op_a, {23{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[24] ? {{8{1'b0}}, op_a, {24{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[25] ? {{7{1'b0}}, op_a, {25{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[26] ? {{6{1'b0}}, op_a, {26{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[27] ? {{5{1'b0}}, op_a, {27{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[28] ? {{4{1'b0}}, op_a, {28{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[29] ? {{3{1'b0}}, op_a, {29{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[30] ? {{2{1'b0}}, op_a, {30{1'b0}}} : {64{1'b0}}) ^
	                        (op_b[31] ? {1'b0, op_a, {31{1'b0}}} : {64{1'b0}});

	// This controls mul_stall which asserts for one cycle only whenever a multiplication
	// operation is present.
	// NOTE: if a load_hazard is present, we delay the update of mul_stall by 1 cycle.
    `ifdef FEATURE_MULT
        reg mul_stall_reg;
        reg [`WORD_WIDTH-1:0] mul_res_reg;
        // initial mul_stall_reg = 0;
        wire is_mul = (ALU_op > 5'd10) & (ALU_op < 5'd15);
        always@(posedge CLK) begin
            if(!nrst) begin 
                mul_stall_reg <= 0;
                mul_res_reg <= 32'd0;
            end
            else if (!load_hazard) begin
                mul_stall_reg <= is_mul & mul_stall;
                if (is_mul) begin
                    if (ALU_op == `ALU_MUL)
                        mul_res_reg <= mul_res[31:0];
                    else
                        mul_res_reg <= mul_res[63:32];
                end
				else
					mul_res_reg <= 32'd0;
            end
        end
        assign mul_stall = ~mul_stall_reg & is_mul;
    `else
        assign mul_stall = 0;
    `endif
	
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
			`ALU_MUL: res = mul_res_reg;
			`ALU_MULHU: res = mul_res_reg;
			`ALU_MULH: res = mul_res_reg;
			`ALU_MULHSU: res = mul_res_reg;
            // division unimplemented
            // Bitmanip crypto extension
            `ALU_ROR: res = (op_a >> op_b[4:0]) | (op_a << (32 - op_b[4:0]));
            `ALU_ROL: res = (op_a << op_b[4:0]) | (op_a >> (32 - op_b[4:0]));
            `ALU_ANDN: res = op_a & ~op_b;
			`ALU_ORN: res = op_a | ~op_b;
			`ALU_XNOR: res = ~(op_a ^ op_b);
			`ALU_PACK: res = {op_b[15:0], op_a[15:0]};
			`ALU_PACKH: res = {16'h0000, op_b[7:0], op_a[7:0]};  
			`ALU_BREV8: res = {op_a[24], op_a[25], op_a[26], op_a[27], op_a[28], op_a[29], op_a[30], op_a[31],
			                   op_a[16], op_a[17], op_a[18], op_a[19], op_a[20], op_a[21], op_a[22], op_a[23],
			                   op_a[8], op_a[9], op_a[10], op_a[11], op_a[12], op_a[13], op_a[14], op_a[15],
			                   op_a[0], op_a[1], op_a[2], op_a[3], op_a[4], op_a[5], op_a[6], op_a[7]};
			`ALU_REV8: res = {op_a[7:0], op_a[15:8], op_a[23:16], op_a[31:24]};
			`ALU_ZIP: res = {op_a[31], op_a[15], op_a[30], op_a[14], op_a[29], op_a[13], op_a[28], op_a[12], op_a[27], op_a[11], op_a[26], op_a[10], op_a[25], op_a[9], op_a[24], op_a[8], op_a[23], op_a[7], op_a[22], op_a[6], op_a[21], op_a[5], op_a[20], op_a[4], op_a[19], op_a[3], op_a[18], op_a[2], op_a[17], op_a[1], op_a[16], op_a[0]};
			`ALU_UNZIP: res = {op_a[31], op_a[29], op_a[27], op_a[25], op_a[23], op_a[21], op_a[19], op_a[17], op_a[15], op_a[13], op_a[11], op_a[9], op_a[7], op_a[5], op_a[3], op_a[1],
			                   op_a[30], op_a[28], op_a[26], op_a[24], op_a[22], op_a[20], op_a[18], op_a[16], op_a[14], op_a[12], op_a[10], op_a[8], op_a[6], op_a[4], op_a[2], op_a[0]};
            `ALU_CLMUL: res = clmul_res[31:0];
            `ALU_CLMULH: res = clmul_res[63:32];
            //other ops 
			default: res = 32'h0;
		endcase
	end
    
endmodule
