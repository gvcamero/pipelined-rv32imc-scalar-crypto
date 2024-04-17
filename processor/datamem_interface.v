`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module datamem_interface(
        input clk,
        input nrst,
        input [2:0] sel_data,           // Writeback data type -- check in lieu of explicit load signal
        input mem_flush,
        // Pass-through to Datamem
        input [`DATAMEM_BITS-1:0] addr_in,
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
        output dm_ready
        /*
        // Control Outputs
        output dm_stall,
        // Datamem-Specifc I/O
        output dm_clk,
        output dm_nrst,
        output [`DATAMEM_BITS-1:0] dm_addr,
        output dm_en
        */
    ); 
    
    reg [31:0] lb_data_reg;
    
    wire [31:0] sb_data_t;
    reg [31:0] sb_data_reg;
    assign sb_data = sb_data_reg;
    wire [3:0] sb_dm_write_t;
    reg [3:0] sb_dm_write_reg;
    assign sb_dm_write = sb_dm_write_reg;
    reg [`DATAMEM_BITS-1:0] addr_out_reg;
    assign addr_out = addr_out_reg;
    reg dm_en_reg;
    assign dm_req = dm_en_reg;
    
    reg [1:0] state;
    assign dm_stall = (sb_is_stype || sel_data == 3'd3) && (state != 2'd3);
    reg dm_ready_reg;
    assign dm_ready = dm_ready_reg;
    
    wire is_mem_op = sb_is_stype || sel_data == 3'd3;
    
    
    
    storeblock STOREBLOCK(
		.opB(sb_opB),
		.byte_offset(sb_byte_offset),
		.store_select(sb_store_select),
		.is_stype(sb_is_stype),
		.data(sb_data_t),
		.dm_write(sb_dm_write_t)
	);
	
	loadblock LOADBLOCK(
		.data(lb_data_reg),
		.byte_offset(lb_byte_offset),
		.dm_select(lb_dm_select),
		.loaddata(lb_loaddata)
	);
	
	// DATA
	always@(posedge clk) begin
	   if (!nrst) begin
	       state <= 2'd0;
	       
           sb_data_reg <= 32'h0;
           sb_dm_write_reg <= 4'h0;
           addr_out_reg <= 32'h0;
           dm_en_reg <= 1'b0;
           lb_data_reg <= 32'd0;
           dm_ready_reg <= 1'b0;
       end
       
       else begin
            case(state)
                // No current memory op
                2'd0: begin
                    if (is_mem_op) begin
                        // Issue operation
                        dm_en_reg <= 1'b1;
                        addr_out_reg <= addr_in;
                        
                        // LOADS take precedence
                        if (sel_data == 3'd3) begin
                            state <= 2'd1;
                            sb_data_reg <= 32'h0;
                            sb_dm_write_reg <= 4'h0;
                        end
                        else begin
                            state <= 2'd3;
                            sb_data_reg <= sb_data_t;
                            sb_dm_write_reg <= sb_dm_write_t;
                        end
                    end
                    else begin
                        state <= 2'd0;
                       
                        sb_data_reg <= 32'h0;
                        sb_dm_write_reg <= 4'h0;
                        addr_out_reg <= 32'h0;
                        dm_en_reg <= 1'b0;
                    end
                    lb_data_reg <= 32'd0;
                    dm_ready_reg <= 1'b0;
                end
                
                // Active load op
                2'd1: begin
                   state <= 2'd3;
                   
                   sb_data_reg <= 32'h0;
                   sb_dm_write_reg <= 4'h0;
                   addr_out_reg <= 32'h0;
                   dm_en_reg <= 1'b0;
                   lb_data_reg <= lb_data;
                   dm_ready_reg <= 1'b0;
                end
                
                // Active store op
                // currently unused
                2'd2: begin
                   state <= 2'd0;
                   
                   sb_data_reg <= 32'h0;
                   sb_dm_write_reg <= 4'h0;
                   addr_out_reg <= 32'h0;
                   dm_en_reg <= 1'b0;
                   lb_data_reg <= 32'd0;
                   dm_ready_reg <= 1'b0;
                end
                
                // Finishing op
                2'd3: begin
                   if (!mem_flush)
                        state <= 2'd0;
                   else
                        state <= 2'd3;
                   
                   sb_data_reg <= 32'h0;
                   sb_dm_write_reg <= 4'h0;
                   addr_out_reg <= 32'h0;
                   dm_en_reg <= 1'b0;
                   lb_data_reg <= 32'd0;
                   dm_ready_reg <= 1'b0;
                end
            endcase
       end
    end
endmodule
