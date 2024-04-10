`timescale 1ns / 1ps

`include "constants.vh"
`include "config.vh"

module tb_core_extmem();

    parameter NUM_TESTS = 76;
string test_pile[NUM_TESTS] = '{
	"C-ADD",
	"C-ADDI",
	"C-ADDI16SP",
	"C-ADDI4SPN",
	"C-AND",
	"C-ANDI",
	"C-BEQZ",
	"C-BNEZ",
	"C-J",
	"C-JAL",
	"C-JALR",
	"C-JR",
	"C-LI",
	"C-LUI",
	"C-LW",
	"C-LWSP",
	"C-MV",
	"C-OR",
	"C-SLLI",
	"C-SRAI",
	"C-SRLI",
	"C-SUB",
	"C-SW",
	"C-SWSP",
	"C-XOR",
	"DIV",
	"DIVU",
	"I-ADD-01",
	"I-ADDI-01",
	"I-AND-01",
	"I-ANDI-01",
	"I-AUIPC-01",
	"I-BEQ-01",
	"I-BGE-01",
	"I-BGEU-01",
	"I-BLT-01",
	"I-BLTU-01",
	"I-BNE-01",
	"I-DELAY_SLOTS-01",
	"I-ENDIANESS-01",
	"I-IO-01",
	"I-JAL-01",
	"I-JALR-01",
	"I-LB-01",
	"I-LBU-01",
	"I-LH-01",
	"I-LHU-01",
	"I-LUI-01",
	"I-LW-01",
	"I-NOP-01",
	"I-OR-01",
	"I-ORI-01",
	"I-RF_size-01",
	"I-RF_width-01",
	"I-SB-01",
	"I-SH-01",
	"I-SLL-01",
	"I-SLLI-01",
	"I-SLT-01",
	"I-SLTI-01",
	"I-SLTIU-01",
	"I-SLTU-01",
	"I-SRA-01",
	"I-SRAI-01",
	"I-SRL-01",
	"I-SRLI-01",
	"I-SUB-01",
	"I-SW-01",
	"I-XOR-01",
	"I-XORI-01",
	"MUL",
	"MULH",
	"MULHSU",
	"MULHU",
	"REM",
	"REMU"
};

parameter string instmem_pile[NUM_TESTS] = {
	"instmem-dump/mem/C-ADD.mem",
	"instmem-dump/mem/C-ADDI.mem",
	"instmem-dump/mem/C-ADDI16SP.mem",
	"instmem-dump/mem/C-ADDI4SPN.mem",
	"instmem-dump/mem/C-AND.mem",
	"instmem-dump/mem/C-ANDI.mem",
	"instmem-dump/mem/C-BEQZ.mem",
	"instmem-dump/mem/C-BNEZ.mem",
	"instmem-dump/mem/C-J.mem",
	"instmem-dump/mem/C-JAL.mem",
	"instmem-dump/mem/C-JALR.mem",
	"instmem-dump/mem/C-JR.mem",
	"instmem-dump/mem/C-LI.mem",
	"instmem-dump/mem/C-LUI.mem",
	"instmem-dump/mem/C-LW.mem",
	"instmem-dump/mem/C-LWSP.mem",
	"instmem-dump/mem/C-MV.mem",
	"instmem-dump/mem/C-OR.mem",
	"instmem-dump/mem/C-SLLI.mem",
	"instmem-dump/mem/C-SRAI.mem",
	"instmem-dump/mem/C-SRLI.mem",
	"instmem-dump/mem/C-SUB.mem",
	"instmem-dump/mem/C-SW.mem",
	"instmem-dump/mem/C-SWSP.mem",
	"instmem-dump/mem/C-XOR.mem",
	"instmem-dump/mem/DIV.mem",
	"instmem-dump/mem/DIVU.mem",
	"instmem-dump/mem/I-ADD-01.mem",
	"instmem-dump/mem/I-ADDI-01.mem",
	"instmem-dump/mem/I-AND-01.mem",
	"instmem-dump/mem/I-ANDI-01.mem",
	"instmem-dump/mem/I-AUIPC-01.mem",
	"instmem-dump/mem/I-BEQ-01.mem",
	"instmem-dump/mem/I-BGE-01.mem",
	"instmem-dump/mem/I-BGEU-01.mem",
	"instmem-dump/mem/I-BLT-01.mem",
	"instmem-dump/mem/I-BLTU-01.mem",
	"instmem-dump/mem/I-BNE-01.mem",
	"instmem-dump/mem/I-DELAY_SLOTS-01.mem",
	"instmem-dump/mem/I-ENDIANESS-01.mem",
	"instmem-dump/mem/I-IO-01.mem",
	"instmem-dump/mem/I-JAL-01.mem",
	"instmem-dump/mem/I-JALR-01.mem",
	"instmem-dump/mem/I-LB-01.mem",
	"instmem-dump/mem/I-LBU-01.mem",
	"instmem-dump/mem/I-LH-01.mem",
	"instmem-dump/mem/I-LHU-01.mem",
	"instmem-dump/mem/I-LUI-01.mem",
	"instmem-dump/mem/I-LW-01.mem",
	"instmem-dump/mem/I-NOP-01.mem",
	"instmem-dump/mem/I-OR-01.mem",
	"instmem-dump/mem/I-ORI-01.mem",
	"instmem-dump/mem/I-RF_size-01.mem",
	"instmem-dump/mem/I-RF_width-01.mem",
	"instmem-dump/mem/I-SB-01.mem",
	"instmem-dump/mem/I-SH-01.mem",
	"instmem-dump/mem/I-SLL-01.mem",
	"instmem-dump/mem/I-SLLI-01.mem",
	"instmem-dump/mem/I-SLT-01.mem",
	"instmem-dump/mem/I-SLTI-01.mem",
	"instmem-dump/mem/I-SLTIU-01.mem",
	"instmem-dump/mem/I-SLTU-01.mem",
	"instmem-dump/mem/I-SRA-01.mem",
	"instmem-dump/mem/I-SRAI-01.mem",
	"instmem-dump/mem/I-SRL-01.mem",
	"instmem-dump/mem/I-SRLI-01.mem",
	"instmem-dump/mem/I-SUB-01.mem",
	"instmem-dump/mem/I-SW-01.mem",
	"instmem-dump/mem/I-XOR-01.mem",
	"instmem-dump/mem/I-XORI-01.mem",
	"instmem-dump/mem/MUL.mem",
	"instmem-dump/mem/MULH.mem",
	"instmem-dump/mem/MULHSU.mem",
	"instmem-dump/mem/MULHU.mem",
	"instmem-dump/mem/REM.mem",
	"instmem-dump/mem/REMU.mem"
};
    
    parameter string datamem_pile[NUM_TESTS] = '{
        "datamem-dump/mem/C-ADD.mem",
        "datamem-dump/mem/C-ADDI.mem",
        "datamem-dump/mem/C-ADDI16SP.mem",
        "datamem-dump/mem/C-ADDI4SPN.mem",
        "datamem-dump/mem/C-AND.mem",
        "datamem-dump/mem/C-ANDI.mem",
        "datamem-dump/mem/C-BEQZ.mem",
        "datamem-dump/mem/C-BNEZ.mem",
        "datamem-dump/mem/C-J.mem",
        "datamem-dump/mem/C-JAL.mem",
        "datamem-dump/mem/C-JALR.mem",
        "datamem-dump/mem/C-JR.mem",
        "datamem-dump/mem/C-LI.mem",
        "datamem-dump/mem/C-LUI.mem",
        "datamem-dump/mem/C-LW.mem",
        "datamem-dump/mem/C-LWSP.mem",
        "datamem-dump/mem/C-MV.mem",
        "datamem-dump/mem/C-OR.mem",
        "datamem-dump/mem/C-SLLI.mem",
        "datamem-dump/mem/C-SRAI.mem",
        "datamem-dump/mem/C-SRLI.mem",
        "datamem-dump/mem/C-SUB.mem",
        "datamem-dump/mem/C-SW.mem",
        "datamem-dump/mem/C-SWSP.mem",
        "datamem-dump/mem/C-XOR.mem",
        "datamem-dump/mem/DIV.mem",
        "datamem-dump/mem/DIVU.mem",
        "datamem-dump/mem/I-ADD-01.mem",
        "datamem-dump/mem/I-ADDI-01.mem",
        "datamem-dump/mem/I-AND-01.mem",
        "datamem-dump/mem/I-ANDI-01.mem",
        "datamem-dump/mem/I-AUIPC-01.mem",
        "datamem-dump/mem/I-BEQ-01.mem",
        "datamem-dump/mem/I-BGE-01.mem",
        "datamem-dump/mem/I-BGEU-01.mem",
        "datamem-dump/mem/I-BLT-01.mem",
        "datamem-dump/mem/I-BLTU-01.mem",
        "datamem-dump/mem/I-BNE-01.mem",
        "datamem-dump/mem/I-DELAY_SLOTS-01.mem",
        "datamem-dump/mem/I-ENDIANESS-01.mem",
        "datamem-dump/mem/I-IO-01.mem",
        "datamem-dump/mem/I-JAL-01.mem",
        "datamem-dump/mem/I-JALR-01.mem",
        "datamem-dump/mem/I-LB-01.mem",
        "datamem-dump/mem/I-LBU-01.mem",
        "datamem-dump/mem/I-LH-01.mem",
        "datamem-dump/mem/I-LHU-01.mem",
        "datamem-dump/mem/I-LUI-01.mem",
        "datamem-dump/mem/I-LW-01.mem",
        "datamem-dump/mem/I-NOP-01.mem",
        "datamem-dump/mem/I-OR-01.mem",
        "datamem-dump/mem/I-ORI-01.mem",
        "datamem-dump/mem/I-RF_size-01.mem",
        "datamem-dump/mem/I-RF_width-01.mem",
        "datamem-dump/mem/I-SB-01.mem",
        "datamem-dump/mem/I-SH-01.mem",
        "datamem-dump/mem/I-SLL-01.mem",
        "datamem-dump/mem/I-SLLI-01.mem",
        "datamem-dump/mem/I-SLT-01.mem",
        "datamem-dump/mem/I-SLTI-01.mem",
        "datamem-dump/mem/I-SLTIU-01.mem",
        "datamem-dump/mem/I-SLTU-01.mem",
        "datamem-dump/mem/I-SRA-01.mem",
        "datamem-dump/mem/I-SRAI-01.mem",
        "datamem-dump/mem/I-SRL-01.mem",
        "datamem-dump/mem/I-SRLI-01.mem",
        "datamem-dump/mem/I-SUB-01.mem",
        "datamem-dump/mem/I-SW-01.mem",
        "datamem-dump/mem/I-XOR-01.mem",
        "datamem-dump/mem/I-XORI-01.mem",
        "datamem-dump/mem/MUL.mem",
        "datamem-dump/mem/MULH.mem",
        "datamem-dump/mem/MULHSU.mem",
        "datamem-dump/mem/MULHU.mem",
        "datamem-dump/mem/REM.mem",
        "datamem-dump/mem/REMU.mem"
    };
    
    parameter string ref_mem_pile[NUM_TESTS] = '{
        "answer-keys/mem/C-ADD.mem",
        "answer-keys/mem/C-ADDI.mem",
        "answer-keys/mem/C-ADDI16SP.mem",
        "answer-keys/mem/C-ADDI4SPN.mem",
        "answer-keys/mem/C-AND.mem",
        "answer-keys/mem/C-ANDI.mem",
        "answer-keys/mem/C-BEQZ.mem",
        "answer-keys/mem/C-BNEZ.mem",
        "answer-keys/mem/C-J.mem",
        "answer-keys/mem/C-JAL.mem",
        "answer-keys/mem/C-JALR.mem",
        "answer-keys/mem/C-JR.mem",
        "answer-keys/mem/C-LI.mem",
        "answer-keys/mem/C-LUI.mem",
        "answer-keys/mem/C-LW.mem",
        "answer-keys/mem/C-LWSP.mem",
        "answer-keys/mem/C-MV.mem",
        "answer-keys/mem/C-OR.mem",
        "answer-keys/mem/C-SLLI.mem",
        "answer-keys/mem/C-SRAI.mem",
        "answer-keys/mem/C-SRLI.mem",
        "answer-keys/mem/C-SUB.mem",
        "answer-keys/mem/C-SW.mem",
        "answer-keys/mem/C-SWSP.mem",
        "answer-keys/mem/C-XOR.mem",
        "answer-keys/mem/DIV.mem",
        "answer-keys/mem/DIVU.mem",
        "answer-keys/mem/I-ADD-01.mem",
        "answer-keys/mem/I-ADDI-01.mem",
        "answer-keys/mem/I-AND-01.mem",
        "answer-keys/mem/I-ANDI-01.mem",
        "answer-keys/mem/I-AUIPC-01.mem",
        "answer-keys/mem/I-BEQ-01.mem",
        "answer-keys/mem/I-BGE-01.mem",
        "answer-keys/mem/I-BGEU-01.mem",
        "answer-keys/mem/I-BLT-01.mem",
        "answer-keys/mem/I-BLTU-01.mem",
        "answer-keys/mem/I-BNE-01.mem",
        "answer-keys/mem/I-DELAY_SLOTS-01.mem",
        "answer-keys/mem/I-ENDIANESS-01.mem",
        "answer-keys/mem/I-IO-01.mem",
        "answer-keys/mem/I-JAL-01.mem",
        "answer-keys/mem/I-JALR-01.mem",
        "answer-keys/mem/I-LB-01.mem",
        "answer-keys/mem/I-LBU-01.mem",
        "answer-keys/mem/I-LH-01.mem",
        "answer-keys/mem/I-LHU-01.mem",
        "answer-keys/mem/I-LUI-01.mem",
        "answer-keys/mem/I-LW-01.mem",
        "answer-keys/mem/I-NOP-01.mem",
        "answer-keys/mem/I-OR-01.mem",
        "answer-keys/mem/I-ORI-01.mem",
        "answer-keys/mem/I-RF_size-01.mem",
        "answer-keys/mem/I-RF_width-01.mem",
        "answer-keys/mem/I-SB-01.mem",
        "answer-keys/mem/I-SH-01.mem",
        "answer-keys/mem/I-SLL-01.mem",
        "answer-keys/mem/I-SLLI-01.mem",
        "answer-keys/mem/I-SLT-01.mem",
        "answer-keys/mem/I-SLTI-01.mem",
        "answer-keys/mem/I-SLTIU-01.mem",
        "answer-keys/mem/I-SLTU-01.mem",
        "answer-keys/mem/I-SRA-01.mem",
        "answer-keys/mem/I-SRAI-01.mem",
        "answer-keys/mem/I-SRL-01.mem",
        "answer-keys/mem/I-SRLI-01.mem",
        "answer-keys/mem/I-SUB-01.mem",
        "answer-keys/mem/I-SW-01.mem",
        "answer-keys/mem/I-XOR-01.mem",
        "answer-keys/mem/I-XORI-01.mem",
        "answer-keys/mem/MUL.mem",
        "answer-keys/mem/MULH.mem",
        "answer-keys/mem/MULHSU.mem",
        "answer-keys/mem/MULHU.mem",
        "answer-keys/mem/REM.mem",
        "answer-keys/mem/REMU.mem"
    };
	
	reg CLK;
	reg nrst[0:NUM_TESTS-1];

	reg [`INT_SIG_WIDTH-1:0] int_sig;

	reg [3:0] con_write;
	reg [`DATAMEM_BITS-1:0] con_addr [0:NUM_TESTS-1];
	reg [`WORD_WIDTH-1:0] con_in;
	wire [`WORD_WIDTH-1:0] con_out [0:NUM_TESTS-1];

	wire [`WORD_WIDTH:0] INST [0:NUM_TESTS];
	reg [`WORD_WIDTH:0] last_inst [0:NUM_TESTS];
	
	always
	   #10 CLK = ~CLK;		// 50MHz clock
    
		
	

	
	
	
	reg [31:0] nop_counter [0:NUM_TESTS];
	// Checking for 10 NOPs/50 looping jumps in a row
    // NOTE: checking for last_inst should be done for at least 50 cycles
    // if there are DIV operations running in the processor.
    wire [31:0] box[0:NUM_TESTS-1];
    
    reg suite_done [0:NUM_TESTS-1];
	
	for (genvar i_f=0; i_f < NUM_TESTS; i_f++) begin
        localparam string temp_data = $sformatf("%s%s%s", `REPO_LOCATION, `TEST_LOCATION, datamem_pile[i_f]);
        localparam string temp_inst = $sformatf("%s%s%s", `REPO_LOCATION, `TEST_LOCATION, instmem_pile[i_f]);
        localparam string temp_refm = $sformatf("%s%s%s", `REPO_LOCATION, `TEST_LOCATION, ref_mem_pile[i_f]);
        
        // Integers for checking results through the answer key
        integer i, j, check, done, pass, consecutive_nops;
        integer total_test_cases = 0;
        integer print_metrics = 0;
        
        // Tracking "highest" data address written to for
        // displaying only what's needed in the answer key
        // (since there is no need to display the addresses
        // not written to)
        integer max_data_addr;
        
        wire [3:0] core_data_write;
        wire [`DATAMEM_BITS-1:0] core_data_addr;	
        wire [`DATAMEM_WIDTH-1:0] core_data_store;	
        wire [`DATAMEM_WIDTH-1:0] core_data_load;
        wire core_data_en;
        
        wire [`PC_ADDR_BITS-1:0] core_inst_addr;
	    wire [`WORD_WIDTH-1:0] core_inst_data;
        
        datamem #(
            .INITIAL_DATA(temp_data)
        ) DATAMEM (
            .core_clk(CLK),
            .con_clk(CLK),
            .nrst(nrst[i_f]),
    
            .dm_write(core_data_write),
            .data_addr(core_data_addr),        
            .data_in(core_data_store),
            .data_en(core_data_en),
    
            .con_write(con_write),
            .con_addr(con_addr[i_f]),
            .con_in(con_in),
            .con_en(1'b1),
    
            .data_out(core_data_load),
            .con_out(con_out[i_f])
        );
        
        instmem #(
           .INSTMEM_PROGRAM(temp_inst)
        ) INSTMEM (
            .sel_ISR(1'b0),
    
            .addr(core_inst_addr),
            .inst(core_inst_data)
        );
        
        core_extmem #() CORE(
            .CLKIP_OUT(CLK),
            .CLK_BUF(CLK),
            .nrst(nrst[i_f]),
    
            .int_sig(int_sig),
            
            .ext_data_write(core_data_write),
            .ext_data_addr(core_data_addr),        
            .ext_data_store(core_data_store),
            .ext_data_en(core_data_en),
            .ext_data_load(core_data_load),
            
            .ext_inst_addr(core_inst_addr),
	        .ext_inst_data(core_inst_data)
        );
        answerkey_i #(.REF_OUT(temp_refm)) AK();
        
        assign INST[i_f] = CORE.if_inst;
        
        assign box[i_f] = {AK.memory[con_addr[i_f]][7:0], AK.memory[con_addr[i_f]][15:8], AK.memory[con_addr[i_f]][23:16], AK.memory[con_addr[i_f]][31:24]};
        
        always@(posedge CLK) begin
            if (!nrst[i_f]) begin
                check = 0;
                consecutive_nops = 0;
                last_inst[i_f] = 0;
            end
            else
            if (!done)
                if ((last_inst[i_f][15:0] == 16'h0001 || last_inst[i_f] == 32'h13) && (INST[i_f][15:0] == 16'h0001 || INST[i_f] == 32'h13)) begin
                    consecutive_nops = consecutive_nops + 1;
                    check = check + 1;
                end
                else if (INST[i_f] == last_inst[i_f]) begin
                    check = check + 1;
                end
                else begin
                    last_inst[i_f] <= INST[i_f];
                    consecutive_nops = 0;
                    check = 0;
                end
        end
 
        initial begin
            CLK = 0;
            nrst[i_f] = 0;
    
            int_sig = 0;
            // BTN = 0;
            // SW = 0;
            last_inst[i_f] = 0;
    
            con_write = 0;
            con_addr[i_f] = 10'h0;
            con_in = 0;
    
            done = 0;
            check = 0;
            pass = 0;
            i = 0;
            j = 0;
            suite_done[i_f] = 0;
            
            if (i_f == 0)  
                #100 nrst[i_f] = 1;
        end
        
        if (i_f != 0) begin
            always@(posedge suite_done[i_f-1]) begin
                nrst[i_f] = 1;
            end
        end
        
        // This controls max_data_addr
        always@(posedge CLK) begin
            if(!nrst[i_f])
                max_data_addr <= 0;
            else if(!done) 
                if((CORE.exe_is_stype && CORE.exe_dm_write && CORE.exe_ALUout[12:2] > max_data_addr) && (CORE.exe_ALUout[12:2] < 11'h400)) begin
                    if (max_data_addr > 255) begin
                        max_data_addr = 256;
                    end
                    else begin
                        max_data_addr <= CORE.exe_ALUout[12:2];
                    end
                end
        end
        
       // This controls the NOP counter
        always@(posedge CLK) begin
           if (!done)
                if(!nrst[i_f])
                    nop_counter[i_f] <= 0;
                else if(!done)
                    if(INST[i_f][15:0] == 16'h0001 || INST[i_f] == 32'h00000013)
                        nop_counter[i_f] <= nop_counter[i_f] + 1;
        end
        // This controlls the done flag
        always@(posedge CLK) begin
            if(check == 50 || consecutive_nops == 8) done = 1;
        end
        
       // The following code is for checking the contents
        // of BLOCKMEM
        always@(posedge done) begin
            
            $display("%s", test_pile[i_f]);
            $display("---------| SUMMARY |---------");
            $display("Address\t  Actual  \tExpected ");
            $display("=======\t==========\t==========");	
        end
    
        always@(negedge CLK) begin
            if(done) begin	
                if(con_out[i_f] == box[i_f]) begin
                    //$display("0x%3X\t0x%X\t0x%X\tPass", con_addr, con_out, AK.memory[con_addr]);
                    pass = pass + 1;
                end else begin
                    if (!print_metrics) begin
                        $display("0x%3X\t0x%X\t0x%X\tFail--------------------", con_addr[i_f], con_out[i_f], box[i_f]);
                    end
                end
    
                total_test_cases = total_test_cases + 1;
                con_addr[i_f] = con_addr[i_f] + 1;
                if(con_addr[i_f] == max_data_addr) print_metrics = 1;
            end
        end
        
        always@(posedge print_metrics) begin
            $display("Passed %0d/%0d test cases.\n\n", pass, total_test_cases);
            suite_done[i_f] = 1;
            if (i_f == NUM_TESTS - 1)
                $finish;
        end
	end
endmodule

// ANSWER KEY
module answerkey #(parameter REF_OUT = "answerkey.mem")();
	reg [31:0] memory [0:`DATAMEM_DEPTH-1];
	initial begin
	    for (int i = 0; i < `DATAMEM_DEPTH-1; i++) begin
	       memory[i] = 32'd0;
	    end
		$readmemh(REF_OUT, memory);
	end
endmodule