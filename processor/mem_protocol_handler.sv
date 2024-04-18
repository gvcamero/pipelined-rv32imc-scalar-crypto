`timescale 1ns / 1ps
`include "constants.vh"
`include "config.vh"

module mem_protocol_handler #(
    parameter GRANT_DELAY = 0,              // delay before granting memory access
    parameter VALID_DELAY = 0              // delay before read operation finishes after grant
    )(
    input clk,
    input nrst,
    
    input [`DATAMEM_BITS-1:0] addr_in,
    output reg [`DATAMEM_BITS-1:0] addr,
    
    input [`WORD_WIDTH-1:0] read_in,
    output reg [`WORD_WIDTH-1:0] read,
    
    input [3:0] wren, 
    output reg op,
    output reg [3:0] wren_out,
           
    input req,
    output reg gnt,
    output reg valid
    );
    
    reg [`DATAMEM_BITS-1:0] addr_buffer;
    reg op_buffer;
    reg [`WORD_WIDTH-1:0] read_buffer;
    reg [3:0] wren_buffer;
    
    reg [7:0] gnt_delay_r, val_delay_r;
    reg [7:0] sim_delay;
    reg [2:0] mem_state;
    
    localparam MEM_START = 3'h0;
    localparam MEM_WAIT = 3'h1;
    localparam MEM_GRANT = 3'h2;
    localparam MEM_RESP_READ = 3'h3;
    localparam MEM_RESP_WRITE = 3'h7;
    
    write op_type = (wren != 3'd0);
    
    localparam OP_WRITE = 1'b1;
    localparam OP_READ = 1'b0;
    
    initial begin
        if (GRANT_DELAY < 0)
            gnt_delay_r = 0;
        else if (GRANT_DELAY > 8'h7F)
            gnt_delay_r = 8'h7F;
        else
            gnt_delay_r = GRANT_DELAY;  
            
        if (VALID_DELAY < 0)
            val_delay_r = 0;
        else if (GRANT_DELAY > 8'h7F)
            val_delay_r = 8'h7F;
        else
            val_delay_r = VALID_DELAY;    
    end
    
    always_comb begin
        if (val_delay_r == 0) begin
            // Ideal memory
            read = read_in;
            wren_out = 0;
            if (mem_state == MEM_START) begin
                addr = addr_in;
                op = op_type;           // redo
            end
            else begin
                addr = addr_buffer;
                op = op_buffer;         // redo
            end     
        end
        else begin
            addr = addr_buffer;
            op = op_buffer;             // redo
            read = read_buffer;
            if (mem_state == MEM_RESP_WRITE)
                wren_out = wren_buffer;
            else
                wren_out = 0;
        end
    end
    
    always@(posedge clk) begin
        if (!nrst) begin
            addr_buffer <= 0;
            op_buffer <= 0;
            read_buffer <= 0;
            wren_buffer <= 4'd0;
            
            gnt <= 0;
            valid <= 0;
        
            sim_delay <= 8'h0;
            mem_state <= MEM_START;
        end
        else begin
            case(mem_state)
                MEM_START: begin
                    if (req) begin
                        // try request
                        addr_buffer <= addr_in;
                        op_buffer <= op_type;
                        wren_buffer <= wren;
                        read_buffer <= 0;
                        
                        sim_delay <= sim_delay + 1;
                        
                        if (gnt_delay_r == 0) begin
                            // instant grant
                            gnt <= 1;
                            if (op_type == OP_WRITE) begin
                                // issue write now
                                valid <= 0;
                                mem_state <= MEM_RESP_WRITE;
                            end
                            else begin
                                if (val_delay_r == 0) begin
                                    // issue read now
                                    valid <= 1;
                                    mem_state <= MEM_RESP_READ;
                                end
                                else begin
                                    // wait for valid read
                                    valid <= 0;
                                    mem_state <= MEM_GRANT;
                                end
                            end
                        end
                        else begin
                            // wait for grant
                            gnt <= 0;
                            valid <= 0;
                            mem_state <= MEM_WAIT;
                        end
                    end
                    
                    else begin
                        // no request = reset
                        addr_buffer <= 0;
                        op_buffer <= 0;
                        read_buffer <= 0;
                        wren_buffer <= 0;
                        
                        gnt <= 0;
                        valid <= 0;
                        
                        sim_delay <= 8'h0;
                        mem_state <= MEM_START;
                    end
                end
                
                MEM_WAIT: begin
                    // check if request is still active
                    if (req) begin
                        sim_delay <= sim_delay + 1;
                        
                        addr_buffer <= addr_buffer;
                        op_buffer <= op_buffer;
                        wren_buffer <= wren_buffer;
                        read_buffer <= 0;
                        
                        if (sim_delay > gnt_delay_r) begin
                            // Grant request
                            gnt <= 1;
                            if (val_delay_r == 0) begin
                                if (op_buffer == OP_WRITE) begin
                                    valid <= 0;
                                    mem_state <= MEM_RESP_WRITE;
                                end
                                else begin
                                    valid <= 1;
                                    mem_state <= MEM_RESP_READ;
                                end 
                            end
                            else begin
                                valid <= 0;
                                mem_state <= MEM_GRANT;
                            end
                        end
                        else begin
                            // Keep waiting
                            valid <= 0;
                            gnt <= 0;
                            mem_state <= MEM_WAIT;
                        end
                    end
                    else begin
                        // no request = reset
                        addr_buffer <= 0;
                        op_buffer <= 0;
                        read_buffer <= 0;
                        wren_buffer <= 0;
                        
                        gnt <= 0;
                        valid <= 0;
                        
                        sim_delay <= 8'h0;
                        mem_state <= MEM_START;
                    end
                end
                
                MEM_GRANT: begin
                    // No takebacks! Request will push through
                    // Assume req signal does not deassert on grant switch
                    sim_delay <= sim_delay + 1;
                    addr_buffer <= addr_buffer;
                    op_buffer <= op_buffer;
                    wren_buffer <= 0;
                    
                    gnt <= 0;
                    
                    if (sim_delay > gnt_delay_r + val_delay_r) begin
                        valid <= 1;
                        mem_state <= MEM_RESP_READ;    
                    end
                    else begin
                        valid <= 0;
                        mem_state <= MEM_GRANT;
                    end
                end
                
                MEM_RESP_READ: begin
                    // 1 cycle response and cleanup for reads
                    addr_buffer <= 0;
                    op_buffer <= 0;
                    read_buffer <= 0;
                    wren_buffer <= 0;
                    
                    gnt <= 0;
                    valid <= 0;
                    
                    sim_delay <= 8'h0;
                    mem_state <= MEM_START;
                end
                
                MEM_RESP_WRITE: begin
                    // 1 cycle response and cleanup for writes
                    addr_buffer <= 0;
                    op_buffer <= 0;
                    read_buffer <= 0;
                    wren_buffer <= 0;
                    
                    gnt <= 0;
                    valid <= 0;
                    
                    sim_delay <= 8'h0;
                    mem_state <= MEM_START;
                end
                
                default: begin
                    addr_buffer <= 0;
                    op_buffer <= 0;
                    read_buffer <= 0;
                    wren_buffer <= 0;
                        
                    gnt <= 0;
                    valid <= 0;
                
                    sim_delay <= 8'h0;
                    mem_state <= MEM_START;
                end
            endcase
        end
    end
    
endmodule
