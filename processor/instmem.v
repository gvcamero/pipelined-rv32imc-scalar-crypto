//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// instmem.v -- Instruction memory module
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: Microlab 198 Pipelined RISC-V Group (2SAY1920)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: instmem.v
// Description: This module implements the 8kB Instruction memory used by the RISC-V core.
//				4kB each are allocated for the main instmem & ISR ROMs. Both ROMs are
//				halfword addressable for compressed instructions support.
//
// Revisions:
// Revision 0.01 - File Created
// Additional Comments:
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


`timescale 1ns / 1ps

`include "constants.vh"

module instmem (
    input clk,
    input nrst,
	input sel_ISR,

	input [`PC_ADDR_BITS-1:0] addr,
	output [`WORD_WIDTH-1:0] inst
);
	
	wire [`WORD_WIDTH-1:0] prog;
	// wire [`WORD_WIDTH-1:0] isr;
	wire [`WORD_WIDTH-1:0] inst_be;
	wire [`WORD_WIDTH-1:0] inst_t;
	reg [`WORD_WIDTH-1:0] inst_reg;

	reg [`WORD_WIDTH-1:0] instmem [0:`MEM_DEPTH-1];
	
	integer i;
	
	assign prog = instmem[addr[`PC_ADDR_BITS-1:2]];

    assign inst_be = prog; // sel_ISR? isr : prog;
    assign inst_t = {inst_be[7:0], inst_be[15:8], inst_be[23:16], inst_be[31:24]};
    assign inst = inst_t;
    
    always@(posedge clk) begin
        if (!nrst)
            inst_reg <= 32'd0;
        else
            inst_reg <= inst_t;
    end
endmodule