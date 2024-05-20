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
`define DEFAULT_FILL    32'h01000100

module instmem #(
    parameter INSTMEM_PROGRAM = "instmem.mem",
    parameter ISR_PROGRAM = "isr_mem.mem"
    )(
    input clk,
    input nrst,
    
	input ready,
	output valid,

	input [`PC_ADDR_BITS-1:0] addr,
	output [`WORD_WIDTH-1:0] inst
);
	reg [`WORD_WIDTH-1:0] instmem [0:`MEM_DEPTH-1];
	wire [`WORD_WIDTH-1:0] inst_le = instmem[addr];
	wire [`WORD_WIDTH-1:0] inst_be = {inst_le[7:0], inst_le[15:8], inst_le[23:16], inst_le[31:24]}; 
		
	reg [`WORD_WIDTH-1:0] prog_t;
	assign inst = prog_t;
	reg valid_t;
	assign valid = valid_t;
	
	integer i;
	initial begin
	    for (i = 0; i < `MEM_DEPTH; i=i+1) begin
	       instmem[i] = `DEFAULT_FILL;
	    end
		$readmemh(INSTMEM_PROGRAM, instmem);
	end
	
	always@(posedge clk) begin
	   if (!nrst) begin
	       prog_t <= 32'd0;
	       valid_t <= 0;
	   end
	   else begin
	       if (valid) begin
	           prog_t <= inst_be;
	           valid_t <= 1;
	       end
	       else begin
	           prog_t <= 32'd0;
	           valid_t <= 0;
	       end 
	   end
	end
	
endmodule