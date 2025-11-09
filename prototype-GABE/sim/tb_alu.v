`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09.09.2025 19:02:13
// Design Name: 
// Module Name: tb_alu_bitmanip
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

module tb_alu();

    reg CLK;
    reg nrst;
    reg load_hazard;
    
    reg [`WORD_WIDTH-1:0] op_a;
    reg [`WORD_WIDTH-1:0] op_b;
    reg [4:0] ALU_op;
    
    wire [`WORD_WIDTH-1:0] res;
    wire mul_stall;
    wire z;
    wire less;
    wire signed_less;

    alu ALU(

        .CLK(CLK),
        .nrst(nrst),
        .load_hazard(load_hazard),

        .op_a(op_a),
        .op_b(op_b),
        .ALU_op(ALU_op),
        
        .res(res),
        .mul_stall(mul_stall),
        .z(z),
        .less(less),
        .signed_less(signed_less)
    );
    
    always begin
        #10 CLK = ~CLK;
    end
    
    initial begin
    
        CLK = 0;
        
        #10
        
        op_a = 32'hAEC19725;
        //op_b = 32'h0E;
        ALU_op = `ALU_ZIP;
        
        #20
        
        op_a = 32'hFF;
        //op_b = 32'h0E;
        ALU_op = `ALU_ZIP;
        
        #20
        
        op_a = 0;
        op_b = 0;
        ALU_op = 0;
        
        #100
        $finish;
    end

endmodule
