`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module datamem_interface(
        input clk,
        input nrst,
        input [2:0] sel_data,           // Writeback data type -- check in lieu of explicit load signal
        input mem_flush,
        // Pass-through to Datamem
        input [`DATAMEM_BITS-1:0] exe_addr_in,
        input [`DATAMEM_BITS-1:0] mem_addr_in,
        output [`DATAMEM_BITS-1:0] addr_out,        
        // Store Block I/O
        input [31:0] sb_opB,
        input [1:0] sb_byte_offset,
        input [1:0] sb_store_select,
        input sb_is_stype,
        output [31:0] sb_data,
        output [3:0] sb_dm_write,
        // Load Block I/O
        input [31:0] lb_data,    
        input [1:0] lb_byte_offset,    
        input [2:0] lb_dm_select,    
        output [31:0] lb_loaddata,
        
        // Control Outputs
        output dm_stall,
        output dm_req,
        input dm_gnt,
        input dm_valid,
        
        // Core Control I/O
        input [`REGFILE_BITS-1:0] mem_rd,
        input stall,
        output read_ready
    ); 
    
    wire [31:0] lb_data_t;
    wire [31:0] sb_data_t;
    wire [3:0] sb_dm_write_t;
    
    wire is_load = (sel_data == 3'd3) && (mem_rd != 0);
    wire is_mem_op = sb_is_stype || is_load;
    wire [`DATAMEM_BITS-1:0] addr_in = is_load ? mem_addr_in : exe_addr_in;
    /*
    reg hold;
    
    always@(posedge clk) begin
        if (!nrst)
            hold <= 0;
        else if (dm_stall)
            hold <= 1;
        else
            hold <= 0;
    end
    */
        
    storeblock STOREBLOCK(
		.opB(sb_opB),
		.byte_offset(sb_byte_offset),
		.store_select(sb_store_select),
		.is_stype(sb_is_stype),
		.load_in_mem(is_load),
		.data(sb_data_t),
		.dm_write(sb_dm_write_t)
	);
	
	loadblock LOADBLOCK(
		.data(lb_data_t),
		.byte_offset(lb_byte_offset),
		.dm_select(lb_dm_select),
		.loaddata(lb_loaddata)
	);

    mem_protocol_driver MPH(
        .clk(clk),
        .nrst(nrst),
        
        .issue_op(is_mem_op), // && ~hold),
        .busy(dm_stall),
        .ready(read_ready),
        
        .issue_addr(addr_in),
        .addr_out(addr_out),
        
        .read_in(lb_data),
        .load_out(lb_data_t),
        
        .store_in(sb_data_t),
        .write_out(sb_data),

        .wren(sb_dm_write_t),
        .wren_out(sb_dm_write),

        .req(dm_req),
        .gnt(dm_gnt),
        .valid(dm_valid)
    );
endmodule
