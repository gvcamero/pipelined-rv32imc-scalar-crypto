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
    
    
    /*
        Types of instruction ops:
        
        (Unfilled buffer)
        - Issue Request
        - Wait for Ready
        - Read, check compressed, issue instruction, fill buffer
        
        (Filled buffer)
        - Check buffer compressed [, issue instruction OR issue request]
        [- Wait for Ready]
        [- Read, issue instruction, fill buffer]
        
        (Jump/Branch)
        - Check alignment, Issue Request
        - Wait for Ready
            (Aligned)
            - Read, check compressed, issue instruction, fill buffer
            (Misaligned)
            - Check buffer compressed [, issue instruction OR issue request]
            [- Wait for Ready]
            [- Read, issue instruction, fill buffer]
    */
    
    /*
        Decoupled processes:
        
        REQUEST
        
        [MEMORY DRIVER]
        
        BUFFER, ISSUE INSTRUCTIONS
    */
    
    // REQUEST [INPUT]
    
    reg [`PC_ADDR_BITS-1:0] inst_addr_t;
    wire issue_req = !id_stall;
    // push effective address into driver

    always@(posedge clk) begin
        if (!nrst) begin
            pc <= '0;
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
    
    // MEMORY INTERFACE [DRIVER]
    
    mem_protocol_driver IPD (
        .clk(clk),
        .nrst(nrst),
        
        .issue_op(issue_req),
        .busy(busy),
        .ready(ready),
        
        .issue_addr(inst_addr_t),
        .addr_out(inst_addr),
        
        .read_in(inst_data),
        .load_out(inst_data_t),
        
        .store_in(32'd0),
        .write_out(),

        .wren(4'd0),
        .wren_out(),

        .req(im_req),
        .gnt(im_gnt),
        .valid(im_valid)
    );
    
    // DISPATCH [OUTPUT]
    
    reg [2:0] n_state;
    
    localparam I_RESET = 3'd0;
    localparam I_NEXT_N = 3'd1;
    localparam I_NEXT_F = 3'd2;
    localparam I_BRANCH_A = 3'd3;
    localparam I_BRANCH_M1 = 3'd4;
    localparam I_BRANCH_M2 = 3'd5;
    
    always@ (posedge clk) begin
        if (!nrst) begin
            n_state <= 3'd0;
        end
        else begin
            case(n_state)
                I_RESET:
                    n_state <= 3'd0;
                default:
                    n_state <= 3'd0;
            endcase
        end
    end
    
	/*
	always@(posedge clk) begin
	   if (!nrst) begin
	       comp_buffer <= 16'd0;
	       jump_buffer <= 16'd0;
           pc <= 32'd0;
           state <= 3'd0;
           filled <= 1'b0;
           in_branch <= 0;
	   end
	   else begin
            if (id_stall) begin
                comp_buffer <= comp_buffer;
                jump_buffer <= jump_buffer;
                pc <= pc;
                filled <= filled;   
                in_branch <= in_branch;
            end
            else begin
                case(state)
                    // Default state
                    3'd0: begin
                        case ({branch, in_branch})
                            // regular instruction
                            2'b00: begin
                                in_branch <= 0;
                                comp_buffer <= inst_data[31:16];
                                jump_buffer <= 16'd0;
                                pc <= if_pcnew;
                                state <= 3'd0;
                                filled <= (align ^ inst_compressed);
                            end
                            // start control flow instruction
                            2'b10: begin
                                state <= 3'd0;
                                if (align_branch) begin
                                    // Branch into misaligned instruction!
                                    // check lowest two bits of first halfword
                                    if (misalign_in_compressed) begin
                                        // 16-bit instruction
                                        // affected by jump_flush --> resolve on next cycle
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b1; 
                                    end
                                    else begin
                                        pc <= if_pcnew;
                                        in_branch <= 1;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= inst_data[15:0];  
                                        filled <= 1'b1; 
                                    end
                                end
                                else begin
                                    // check lowest two bits instead
                                    if (inst_data[1:0] == 2'd3) begin
                                        // word-aligned 32-bit instruction
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= 16'd0;
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b0;
                                    end
                                    else begin
                                        // word-aligned 16-bit instruction 
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b1; 
                                    end
                                end
                            end
                            // in control flow instruction
                            2'b01: begin
                                comp_buffer <= inst_data[31:16];
                                jump_buffer <= 16'd0;
                                state <= 3'd0;
                                pc <= pc + 32'd4;
                                filled <= 1'b1;
                                in_branch <= 0;
                            end
                            // control flow instruction into another control flow instruction
                            2'b11: begin
                                state <= 3'd0;
                                if (align_branch) begin
                                    // Branch into misaligned instruction!
                                    // check lowest two bits of first halfword
                                    if (misalign_in_compressed) begin
                                        // 16-bit instruction
                                        // affected by jump_flush --> resolve on next cycle
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b1; 
                                    end
                                    else begin
                                        pc <= if_pcnew;
                                        in_branch <= 1;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= inst_data[15:0];  
                                        filled <= 1'b1; 
                                    end
                                end
                                else begin
                                    // check lowest two bits instead
                                    if (inst_data[1:0] == 2'd3) begin
                                        // word-aligned 32-bit instruction
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= 16'd0;
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b0;
                                    end
                                    else begin
                                        // word-aligned 16-bit instruction 
                                        pc <= if_pcnew;
                                        in_branch <= 0;
                                        comp_buffer <= inst_data[31:16];
                                        jump_buffer <= 16'd0;
                                        filled <= 1'b1; 
                                    end
                                end
                            end
                        endcase
                    end
                    // Reset
                    default: begin
                        comp_buffer <= 16'd0;
                        jump_buffer <= 16'd0;
                        pc <= 32'd0;
                        state <= 3'd0;
                        filled <= 1'b0;
                        in_branch <= 0;
                    end
                endcase
            end
        end
	end
	
	// data outputs
    always@(*) begin
        if (branch) begin
            addr_next = if_pcnew;
            inst_out_reg = inst_data;
            // entering control flow instruction
            if (align_branch && !misalign_in_compressed) begin
                // misaligned word fetch needed
                ready_reg = 0;
            end
            else begin
                // no misaligned word fetch
                ready_reg = 1;
            end      
        end
        else if (in_branch) begin
            addr_next = pc + 32'd4;
            ready_reg = 1;
            inst_out_reg = {inst_hi16, inst_lo16};
        end
        
        else begin
            // regular instruction
            if (!align || inst_compressed) begin
                // word-aligned instruction
                addr_next = pc;
                ready_reg = 1;
            end
            else begin
                // non-word-aligned instruction
                if (filled) begin
                    addr_next = pc + 32'd4;
                    ready_reg = 1;
                end
                else begin
                    addr_next = pc;
                    ready_reg = 0;
                end 
            end
            inst_out_reg = {inst_hi16, inst_lo16};
         end
     end
     */
endmodule