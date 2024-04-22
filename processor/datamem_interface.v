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
    
    // States
    localparam STATE_NONE = 3'd0;
    
    localparam STATE_ISSUE_LOAD = 3'd1;
    localparam STATE_ISSUE_STORE = 3'd2;
    
    localparam STATE_ACK_LOAD = 3'd3;
    
    localparam STATE_FINISH = 3'd7; 
    
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
    
    reg [2:0] state;
    assign dm_stall = (sb_is_stype || sel_data == 3'd3) && (state != STATE_FINISH);
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

    mem_protocol_driver MPH(
        .clk(clk),
        .nrst(nrst),
        .issue_op(is_mem_op),
        // .busy(dm_stall),
        .ready()
        .issue_addr(addr_in),
        // .addr_out(addr_out),
        .read_in(lb_data),
        .load_out(),
        .store_in(sb_dm_write_t),
        .store_out(),

        .wren(sb_dm_write),
        .wren_out(),

        .req(dm_req),
        .gnt(dm_gnt),
        .valid(dm_valid)
    );
	
	// DATA
	always@(posedge clk) begin
	   if (!nrst) begin
	       state <= STATE_NONE;
	       
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
                STATE_NONE: begin
                    if (is_mem_op) begin
                        // Issue operation
                        dm_en_reg <= 1'b1;
                        addr_out_reg <= addr_in;
                        
                        // LOADS take precedence
                        if (sel_data == 3'd3) begin
                            state <= STATE_ISSUE_LOAD;
                            sb_data_reg <= 32'h0;
                            sb_dm_write_reg <= 4'h0;
                        end
                        else begin
                            state <= STATE_FINISH;
                            sb_data_reg <= sb_data_t;
                            sb_dm_write_reg <= sb_dm_write_t;
                        end
                    end
                    else begin
                        state <= STATE_NONE;
                       
                        sb_data_reg <= 32'h0;
                        sb_dm_write_reg <= 4'h0;
                        addr_out_reg <= 32'h0;
                        dm_en_reg <= 1'b0;
                    end
                    lb_data_reg <= 32'd0;
                    dm_ready_reg <= 1'b0;
                end
                
                // Active load op
                STATE_ISSUE_LOAD: begin
                   state <= STATE_FINISH;
                   
                   sb_data_reg <= 32'h0;
                   sb_dm_write_reg <= 4'h0;
                   addr_out_reg <= 32'h0;
                   dm_en_reg <= 1'b0;
                   lb_data_reg <= lb_data;
                   dm_ready_reg <= 1'b0;
                end
                
                // Active store op
                // currently unused
                STATE_ISSUE_STORE: begin
                   state <= STATE_NONE;
                   
                   sb_data_reg <= 32'h0;
                   sb_dm_write_reg <= 4'h0;
                   addr_out_reg <= 32'h0;
                   dm_en_reg <= 1'b0;
                   lb_data_reg <= 32'd0;
                   dm_ready_reg <= 1'b0;
                end
                
                // Finishing op
                STATE_FINISH: begin
                   if (!mem_flush)
                        state <= STATE_NONE;
                   else
                        state <= STATE_FINISH;
                   
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
