`timescale 1ns / 1ps

`include "constants.vh"

module instmem_interface_slow (
    input clk,
    input nrst,

    input if_stall,
    input if_flush,
    input id_stall,
    input id_flush,
    
    input [`PC_ADDR_BITS-1:0] if_pc4,
    input [`PC_ADDR_BITS-1:0] if_pcnew,
    input branch,
    
    output [`PC_ADDR_BITS-1:0] if_pc_out,
    output [`PC_ADDR_BITS-1:0] id_pc_out,
    output [`PC_ADDR_BITS-1:0] id_pc4,
    output ready,
    output [`WORD_WIDTH-1:0] if_inst,
    output [`WORD_WIDTH-1:0] id_inst,
    
    input [`WORD_WIDTH-1:0] inst_data,
	output [`PC_ADDR_BITS-1:0] inst_addr
    );

    wire [`PC_ADDR_BITS-1:0] inst_addr_t;
    assign inst_addr = {2'b0, inst_addr_t[`PC_ADDR_BITS-1:2]};
    wire [`WORD_WIDTH-1:0] inst_t = {inst_data[7:0], inst_data[15:8], inst_data[23:16], inst_data[31:24]};

    pipereg_if_id IF_ID(
		.clk(clk),
		.nrst(nrst),

		.flush(id_flush),
		.stall(if_stall),

		.if_pc4(if_pc4), 	.id_pc4(id_pc4),
		.if_inst(if_inst), 	.id_inst(id_inst),
		.if_PC(if_pc_out), 		.id_PC(id_pc_out)
	);

    reg [`WORD_WIDTH-1:0] inst_buffer;
    reg [`WORD_WIDTH/2-1:0] comp_buffer;
    reg [`PC_ADDR_BITS-1:0] issue_addr;
    reg [`PC_ADDR_BITS-1:0] pc_track;
    reg [3:0] state;
    reg ready_reg;

    wire inst_comp = ~&inst_buffer[1:0];
    wire comp_comp = ~&comp_buffer[1:0];

    localparam IM_RESET = 4'h0;
    localparam IM_START_U_A = 4'h1;
    localparam IM_READY_U_A_1 = 4'h2;
    localparam IM_READY_U_A_2 = 4'hA;

    localparam IM_START_F = 4'h3;
    localparam IM_READY_F_1 = 4'h4;
    localparam IM_READY_F_2 = 4'h5;

    localparam IM_START_U_M = 4'h6;
    localparam IM_CHECK_U_M = 4'h7;
    localparam IM_RECHECK_U_M = 4'h8;
    localparam IM_READY_U_M = 4'h9;

    reg [`WORD_WIDTH-1:0] if_inst_t;

    assign inst_addr_t = issue_addr;
    assign if_pc_out = pc_track;
    assign ready = ready_reg;
    assign if_inst = if_inst_t;

    always@(posedge clk) begin
        if (!nrst) begin
            comp_buffer <= 0;
            inst_buffer <= 0;
            issue_addr <= 0;
            pc_track <= 0;
            state <= 4'h0;
            ready_reg <= 0;
        end
        else begin
            if (branch) begin
                // reset
                comp_buffer <= 0;
                inst_buffer <= 0;
                issue_addr <= if_pcnew;
                pc_track <= if_pcnew;
                state <= 4'h0;
                ready_reg <= 0;
            end
            else begin
                case(state)
                    IM_RESET: begin             // clear
                        comp_buffer <= 0;
                        inst_buffer <= 0;
                        issue_addr <= issue_addr;
                        pc_track <= pc_track;
                        if (pc_track[1])            // misaligned start for branches
                            state <= 4'hA;
                        else
                            state <= 4'h1;
                        ready_reg <= 0;
                    end
                    IM_START_U_A: begin             // aligned unfilled buffer start
                        issue_addr <= issue_addr + `PC_ADDR_BITS'h4;       // skip ahead to next word
                        pc_track <= pc_track;
                        state <= IM_READY_U_A_1;
                        ready_reg <= 1;
                        inst_buffer <= inst_t;
                        comp_buffer <= inst_t[31:16];
                    end
                    IM_READY_U_A_1: begin             // aligned unfilled buffer check/ready
                        if (id_stall) begin
                            state <= IM_READY_U_A_1;
                        end
                        else begin
                            if (inst_comp) begin
                                pc_track <= pc_track + `PC_ADDR_BITS'h2;
                                if (comp_comp) begin
                                    state <= IM_READY_U_A_2;
                                    ready_reg <= 1;
                                end
                                else begin
                                    state <= IM_START_F;
                                    ready_reg <= 0;
                                end
                            end
                            else begin
                                pc_track <= pc_track + `PC_ADDR_BITS'h4;
                                state <= IM_START_U_A;
                                ready_reg <= 0;
                            end
                        end
                    end
                    IM_READY_U_A_2: begin
                        if (id_stall) begin
                            state <= IM_READY_U_A_2;
                        end
                        else begin
                            ready_reg <= 0;
                            pc_track <= pc_track + `PC_ADDR_BITS'h2;
                            state <= IM_START_U_A;
                        end
                    end
                    IM_START_F: begin               // filled buffer start
                        if (comp_comp)
                            state <= IM_READY_F_2;
                        else begin
                            state <= IM_READY_F_1;    
                            issue_addr <= issue_addr + `PC_ADDR_BITS'h4;       // skip ahead to next word
                        end
                        pc_track <= pc_track;
                        ready_reg <= 1;
                        inst_buffer <= inst_t;
                        comp_buffer <= comp_buffer;
                    end
                    IM_READY_F_1: begin
                        if (id_stall) begin
                            state <= IM_READY_F_1;
                        end
                        else begin
                            // no compressed instruction in compressed buffer --> check upper half for next instruction
                            comp_buffer <= inst_buffer[31:16];
                            pc_track <= pc_track + `PC_ADDR_BITS'h4;

                            if (comp_comp) begin
                                state <= IM_READY_F_2;
                            end
                            else begin
                                ready_reg <= 0;
                                state <= IM_START_F; 
                            end                            
                        end
                    end
                    IM_READY_F_2: begin
                        if (id_stall) begin
                            state <= IM_READY_F_2;
                        end
                        else begin
                            ready_reg <= 0;
                            pc_track <= pc_track + `PC_ADDR_BITS'h2;
                            state <= IM_START_U_A;
                        end
                    end
                    default: begin
                        // reset
                        comp_buffer <= 0;
                        inst_buffer <= 0;
                        issue_addr <= 0;
                        pc_track <= 0;
                        state <= 4'h0;
                        ready_reg <= 0;
                    end
                endcase
            end
        end
    end

    always@(*) begin
        case(state)
            IM_READY_U_A_1:
                if_inst_t = inst_comp ? {16'h0, inst_buffer[15:0]} : inst_buffer;
            IM_READY_U_A_2:
                if_inst_t = {16'h0, comp_buffer};
            IM_READY_F_1:
                if_inst_t = {inst_buffer[15:0], comp_buffer};
            IM_READY_F_2:
                if_inst_t = {16'h0, comp_buffer};
            default:
                if_inst_t = 32'h0;
        endcase
    end

endmodule