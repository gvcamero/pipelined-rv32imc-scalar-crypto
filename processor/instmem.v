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
	input sel_ISR,

	input [`PC_ADDR_BITS-1:0] addr,
	output [`WORD_WIDTH-1:0] inst
);
	
	wire [`WORD_WIDTH-1:0] prog;
	wire [`WORD_WIDTH-1:0] isr;
	wire [`WORD_WIDTH-1:0] inst_be;
	// wire [`PC_ADDR_BITS-1:0] addr_1 = addr[`PC_ADDR_BITS-1:1];
    // wire [`PC_ADDR_BITS-1:0] addr_2 = addr[`PC_ADDR_BITS-1:1] + 11'd1;

	reg [`WORD_WIDTH-1:0] instmem [0:`MEM_DEPTH-1];
	reg [`WORD_WIDTH-1:0] isr_mem [0:`ISR_DEPTH-1];
	
	integer i;
	initial begin
	    for (i = 0; i < `MEM_DEPTH; i=i+1) begin
	       instmem[i] = `DEFAULT_FILL;
	    end
		$readmemh(INSTMEM_PROGRAM, instmem);
		$readmemh(ISR_PROGRAM, isr_mem);
	end
	assign prog = instmem[addr[`PC_ADDR_BITS-1:2]];
	assign isr = isr_mem[addr[`PC_ADDR_BITS-1:2]];

    assign inst_be = sel_ISR? isr : prog;
    assign inst = {inst_be[7:0], inst_be[15:8], inst_be[23:16], inst_be[31:24]};
endmodule