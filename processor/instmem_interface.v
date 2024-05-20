`timescale 1ns / 1ps

`include "constants.vh"

module instmem_interface (
    input clk,
    input nrst,
    input id_stall,
    input if_flush,
    
    input [`PC_ADDR_BITS-1:0] if_pc4,
    input [`PC_ADDR_BITS-1:0] if_pcnew,
    input branch,
    
    output reg [`PC_ADDR_BITS-1:0] pc,
    output [`WORD_WIDTH-1:0] inst_out,
    
    input [`WORD_WIDTH-1:0] inst_data,
	output [`PC_ADDR_BITS-1:0] inst_addr,
	
	output reg ready_o,                            // can load next instruction
	output reg busy_i,                             // can write next PC 
	
	output im_req,
	input im_gnt,
	input im_valid	
    );
    
    reg [2:0] state;
    reg filled;
    
    reg [`WORD_WIDTH/2-1:0] comp_buffer;
    reg [`WORD_WIDTH/2-1:0] jump_buffer;
    wire align_branch = if_pcnew[1];
    wire align = pc[1];
    reg align_reg;
    
    reg [`PC_ADDR_BITS-1:0] addr_next;
    assign inst_addr = addr_next;
    
    reg in_branch;
    
    reg [`WORD_WIDTH-1:0] inst_out_reg;
    assign inst_out = inst_out_reg;
    wire [15:0] inst_lo16 = align ? comp_buffer : inst_data[15:0];
    wire [15:0] inst_hi16 = align ? inst_data[15:0] : inst_data[31:16]; 
    wire inst_compressed = inst_lo16[1:0] != 2'd3;
    wire misalign_in_compressed = inst_data[17:16] != 2'd3;
    
    wire busy;
    reg issue_req;
        
    reg [`PC_ADDR_BITS-1:0] inst_addr_t;
    wire issue_req = !id_stall;
    // push effective address into driver

    always@(posedge clk) begin
        if (!nrst) begin
            pc <= 32'd0;
        end
        else begin
            if (busy_i) begin
                pc <= pc;  
            end
            else begin
                pc <= if_pcnew;
            end
        end
    end

endmodule

module _pc_o(
    input clk,
    input nrst,
    input hold_i,                                         // take next instruction
    
    input [`PC_ADDR_BITS-1:0] if_pc4,
    input [`PC_ADDR_BITS-1:0] if_pcnew,
    input branch,                                       // control flow instruction
    // input read,
    
    output reg [`PC_ADDR_BITS-1:0] addr_out,            // next instruction location
    output reg [`PC_ADDR_BITS-1:0] pc_out,              // PC for current IF stage instruction
    output reg branch_o,
    output reg hold_o                                     // Hold following instruction(s)
    );
    
    reg [`PC_ADDR_BITS-1:0] pc;
    reg [`PC_ADDR_BITS-1:0] br_buffer;
    reg empty_issue;
    assign hold_o = hold_i;
    
    always@(posedge clk) begin
        if (!nrst) begin
            addr_out <= `PC_ADDR_BITS'd0;
            pc_out <= `PC_ADDR_BITS'd0;
            branch_o <= 0;
            empty_issue <= 1;
        end
        else begin
            addr_out <= if_pcnew;
            pc_out <= addr_out;
            branch_o <= branch;
            empty_issue <= 0;
        end
    end
    
endmodule

module _issue_i(
    input clk,
    input nrst,
    
    input branch,
    input restart,
    input [`PC_ADDR_BITS-1:0] base_addr,
    input [`PC_ADDR_BITS-1:0] restart_addr,
        
    output reg ready,
    output [`WORD_WIDTH-1:0] addr
    );
     
    wire [`PC_ADDR_BITS-1:0] addr_t = !restart ? base_addr : restart_addr;
    assign addr = {{`WORD_WIDTH-`PC_ADDR_BITS{1'b0}}, addr_t};
    
    always@(posedge clk) begin
        if(!nrst) begin
            ready <= 0;
        end
        else begin
            // 1 cycle delay for empty issue clearing
            ready <= !branch;
        end
    end
    
endmodule

module _buffer_o(
    input clk,
    input nrst,

    input hold_in,

    input valid,
    input align,
    input flush,
    input hold,
    input [`WORD_WIDTH-1:0] inst_in,

    output compressed_o,
    output req_d,
    output reg ready_o,
    output reg [`WORD_WIDTH-1:0] inst_out
    );

    reg [`WORD_WIDTH-1:0] inst_buf;
    reg [`WORD_WIDTH-1:0] inst_t;
    reg [15:0] comp_buf;
    reg fill;
    reg hold_r;
    assign compressed_o = align ? comp_buf[1:0] : inst_buf[1:0];

    assign req_d = !fill && align && !compressed_o;

    // Outputs
    always@(*) begin
        if (valid) begin
            inst_out = inst_t;
            ready_o = !req_d;
        end
        else if (hold_r) begin
            inst_out = inst_buf;
            ready_o = 1;
        end
        else begin
            inst_out = 0;
            ready_o = 0;
        end
    end

    // Temp instruction
    always@(*) begin
        if (valid) begin
            if (align)
                inst_t = {comp_buf, inst_in[31:16]};
            else   
                inst_t = inst_in;
        end
        else begin
            inst_t = 0;
        end
    end
    
    // Control + buffer contents
    always@(posedge clk) begin
        if (!nrst) begin
            inst_buf <= 32'd0;
            comp_buf <= 16'd0;
            fill <= 0;
        end
        else begin
            // update for new data coming in
            if (valid) begin
                inst_buf <= inst_t;
                comp_buf <= inst_in[31:16];
                fill <= (align && !compressed_o);
            end
            // Remove contents for branches
            else if (flush) begin
                inst_buf <= 32'd0;
                comp_buf <= 16'd0;
                fill <= 0;
            end
            // hold for now
            else begin
                inst_buf <= inst_buf;
                comp_buf <= comp_buf;
                fill <= fill;
            end
        end
    end

    always@(posedge clk) begin
        if (!nrst)
            hold_r <= 0;
        else
            hold_r <= hold;
    end
endmodule