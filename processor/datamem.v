//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// datamem.v -- Data memory module
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Author: Microlab 198 Pipelined RISC-V Group (2SAY1920)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Module Name: datamem.v
// Description: This module contains the Block Memory Generator IP Modules needed
//				to implement a ~4kB memory for the RISCV processor.
//				Block Memory Generator IP settings for both modules:
//					- Native Interface, True DUAL PORT RAM
//					- Byte write enabled (8bits per byte)
//					- common clock &  generate address unchecked
//					- minimum area algorithm
//					PORT settings (both port a & b)
//						- 32bit write & read width, 1024 write & read depth
//						- Read First operating mode, Always Enabled
//						- checkboxes left unchecked
//					
//
// Revisions:
// Revision 0.01 - File Created
// Additional Comments:
// 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module datamem #(
    parameter INITIAL_DATA = "datamem.mem",
    parameter GRANT_DELAY = 0,              // delay before granting memory access
    parameter VALID_DELAY = 0              // delay before read operation finishes after grant
    )(
	input clk,         		// un-gated clock signal
	input nrst,

	// Inputs from the RISCV core
	input [3:0] dm_write,
	input [`BUS_BITS-1:0] data_addr,
	input [`WORD_WIDTH-1:0] data_in,
	
	// Transactional I/O
	input data_valid,
	output data_gnt,
	output data_req,
	
	// Inputs from protocol controllers
	// NOTE: protocol controllers cannot read from FPGAIO
	input [3:0] con_write,				// Similar to dm_write
	input [`DATAMEM_BITS-1:0] con_addr,	// datamem address from protocol controller
	input [`WORD_WIDTH-1:0] con_in,	// data input from protocol controller
	input con_en,

	// Outputs
	output [`WORD_WIDTH-1:0] data_out,	// data output to the RISC-V core
	output [`WORD_WIDTH-1:0] con_out		// data output to protocol controller
);
	
	// Block memory outputs
	wire [`WORD_WIDTH-1:0] coremem_douta, coremem_doutb;
	wire [`WORD_WIDTH-1:0] protocolmem_douta, protocolmem_doutb;

	// Determine which blockmem output to select
	// If x_sel = 1, select PROTOCOLMEM output, else select COREMEM output
	wire core_sel = data_addr[`DATAMEM_BITS-1];
	wire protocol_sel = con_addr[`DATAMEM_BITS-1];
	
	// Inputs are big-endian words
	// This part converts them to little-endian format
	wire [`WORD_WIDTH-1:0] data_in_little_e = {data_in[7:0], data_in[15:8], data_in[23:16], data_in[31:24]};
	wire [`WORD_WIDTH-1:0] con_in_little_e = {con_in[7:0], con_in[15:8], con_in[23:16], con_in[31:24]};
	
    `ifdef FEATURE_XILINX_DATAMEM_IP_GEN
	// Datamem that uses BLOCKMEM from Vivado IP Catalog
	// Blockmem generated as TRUE DUAL PORT RAM
	// Synchronous read
	// Addresses 0x000 - 0xFFF (Word-aligned addresses)
	blk_mem_gen_datamem COREMEM(
		.clka(clk),
		.wea(dm_write),
		.addra(data_addr[`BUS_BITS-1:2]),
		.dina(data_in_little_e),
		.douta(coremem_douta),

		.clkb(clk),
		.web(4'b0),
		.addrb(con_addr[`DATAMEM_BITS-1:0]),
		.dinb(32'b0),
		.doutb(coremem_doutb)
	);

	// Addresses 0x1000 - 0x100F	(Word-aligned addresses)
	blk_mem_gen_protocol PROTOCOLMEM(
		.clka(clk),
		.wea(4'b0),
		.addra(data_addr[3:0]),
		.dina(32'b0),
		.douta(protocolmem_douta),

		.clkb(clk),
		.web(con_write),
		.addrb(con_addr[3:0]),
		.dinb(con_in_little_e),
		.doutb(protocolmem_doutb)
	);
	
	assign data_gnt = 1;
	assign data_valid = 1;
	
	`else

	wire [`DATAMEM_BITS-1:0] data_addr_t;
	wire [`WORD_WIDTH-1:0] data_write_t;
	wire [`WORD_WIDTH-1:0] data_out_t;
	wire [3:0] data_wren_t;
	wire [3:0] data_wren = dm_write;

	// Manual dual-port RAM
	dual_port_ram_bytewise_write #(
	   .INITIAL_DATA(INITIAL_DATA)
	) COREMEM (
		.clkA(clk),
		.enaA(1'b1),
		.weA(data_wren_t),
		.addrA(data_addr_t[`DATAMEM_BITS-1:0]),
		.dinA(data_write_t),
		.doutA(coremem_douta),

		.clkB(clk),
		.enaB(1'b1),
		.weB(4'b0),
		.addrB(con_addr[`DATAMEM_BITS-2:0]),
		.dinB(32'b0),
		.doutB(coremem_doutb)
	);
	
	dual_port_ram_bytewise_write #(
	   .INITIAL_DATA("answerkey.mem"),
	   .ADDR_WIDTH(4)
	) PROTOCOLMEM(
		.clkA(clk),
		.enaA(1'b1),
		.weA(4'b0),
		.addrA(data_addr_t[3:0]),
		.dinA(32'b0),
		.doutA(protocolmem_douta),

		.clkB(clk),
		.enaB(1'b1),
		.weB(con_write),
		.addrB(con_addr[3:0]),
		.dinB(con_in_little_e),
		.doutB(protocolmem_doutb)
	);
	   
	   
    mem_protocol_handler #(
        .GRANT_DELAY(GRANT_DELAY),
        .VALID_DELAY(VALID_DELAY)
    ) DM_Handler (
        .clk(clk),
        .nrst(nrst),
        
        .addr_in(data_addr[`BUS_BITS-1:2]),
        .addr(data_addr_t),
        
        .read_in(data_out),
        .read(data_out_t),

		.write_in(data_in_little_e),
		.write(data_write_t),
        
        .wren_in(data_wren),
        .wren(data_wren_t),
        
        .req(data_req),
        .gnt(data_gnt),
        .valid(data_valid)
    );
	   
	`endif
	
	// Other Peripherals
	reg [`WORD_WIDTH-1:0] num_cycles = `WORD_WIDTH'd0;               // num_cycles """SFR""" (0x410)
	wire [`WORD_WIDTH-1:0] num_cycles_out = {num_cycles[7:0], num_cycles[15:8], num_cycles[23:16], num_cycles[31:24]};
	reg num_cycles_addr_reg = 1'b0;

	// Assigning data_out for the Core
	reg core_sel_reg = 0;
	always@(posedge clk) begin
		if(!nrst) begin
		      core_sel_reg <= 0;
		      num_cycles_addr_reg <= 1'b0;
		end
		else begin
		      core_sel_reg <= core_sel;
		      num_cycles_addr_reg <= (data_addr == 14'h2010);
		end
	end
	assign data_out = core_sel_reg ?  
	                  ( num_cycles_addr_reg ? num_cycles_out : protocolmem_douta) 
	                  : coremem_douta;

	// Assigning con_out
	reg protocol_sel_reg = 0;
	always@(posedge clk) begin
		if(!nrst) begin
		      protocol_sel_reg <= 0;
		      num_cycles <= `WORD_WIDTH'd0;
		end
		else begin
		      protocol_sel_reg <= protocol_sel;
		      num_cycles <= num_cycles + `WORD_WIDTH'd1;
	    end
	end
	wire [`WORD_WIDTH-1:0] con_out_little_e = protocol_sel_reg? protocolmem_doutb : coremem_doutb;
	assign con_out = {con_out_little_e[7:0], con_out_little_e[15:8], con_out_little_e[23:16], con_out_little_e[31:24]};
endmodule