`timescale 1ns / 1ps

`include "constants.vh"
`include "config.vh"

module tb_core_extmem();

    parameter NUM_TESTS = 70;
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
        "MULHU"
    };
    string current_test = "";
    parameter string file_pile[NUM_TESTS] = {
        "C-ADD.mem",
        "C-ADDI.mem",
        "C-ADDI16SP.mem",
        "C-ADDI4SPN.mem",
        "C-AND.mem",
        "C-ANDI.mem",
        "C-BEQZ.mem",
        "C-BNEZ.mem",
        "C-J.mem",
        "C-JAL.mem",
        "C-JALR.mem",
        "C-JR.mem",
        "C-LI.mem",
        "C-LUI.mem",
        "C-LW.mem",
        "C-LWSP.mem",
        "C-MV.mem",
        "C-OR.mem",
        "C-SLLI.mem",
        "C-SRAI.mem",
        "C-SRLI.mem",
        "C-SUB.mem",
        "C-SW.mem",
        "C-SWSP.mem",
        "C-XOR.mem",
        "I-ADD-01.mem",
        "I-ADDI-01.mem",
        "I-AND-01.mem",
        "I-ANDI-01.mem",
        "I-AUIPC-01.mem",
        "I-BEQ-01.mem",
        "I-BGE-01.mem",
        "I-BGEU-01.mem",
        "I-BLT-01.mem",
        "I-BLTU-01.mem",
        "I-BNE-01.mem",
        "I-DELAY_SLOTS-01.mem",
        "I-JAL-01.mem",
        "I-JALR-01.mem",
        "I-LB-01.mem",
        "I-LBU-01.mem",
        "I-LH-01.mem",
        "I-LHU-01.mem",
        "I-LUI-01.mem",
        "I-LW-01.mem",
        "I-NOP-01.mem",
        "I-OR-01.mem",
        "I-ORI-01.mem",
        "I-RF_size-01.mem",
        "I-RF_width-01.mem",
        "I-SB-01.mem",
        "I-SH-01.mem",
        "I-SLL-01.mem",
        "I-SLLI-01.mem",
        "I-SLT-01.mem",
        "I-SLTI-01.mem",
        "I-SLTIU-01.mem",
        "I-SLTU-01.mem",
        "I-SRA-01.mem",
        "I-SRAI-01.mem",
        "I-SRL-01.mem",
        "I-SRLI-01.mem",
        "I-SUB-01.mem",
        "I-SW-01.mem",
        "I-XOR-01.mem",
        "I-XORI-01.mem",
        "MUL.mem",
        "MULH.mem",
        "MULHSU.mem",
        "MULHU.mem"
    };
	
	reg CLK;
	reg nrst;
	wire [`INT_SIG_WIDTH-1:0] int_sig = 0;;

	reg [3:0] con_write;
	reg [`DATAMEM_BITS-1:0] con_addr;
	reg [`WORD_WIDTH-1:0] con_in;
	wire [`WORD_WIDTH-1:0] con_out;

	wire [`WORD_WIDTH:0] INST;
	reg [`WORD_WIDTH:0] last_inst;
	
	always
	   #10 CLK = ~CLK;		// 50MHz clock
    
		
	

	
	
	
	reg [31:0] nop_counter;
	// Checking for 10 NOPs/50 looping jumps in a row
    // NOTE: checking for last_inst should be done for at least 50 cycles
    // if there are DIV operations running in the processor.
    wire [31:0] box;
    
    // Integers for checking results through the answer key
    integer i, j, check, done, pass, consecutive_nops;
    integer total_test_cases = 0;
    integer print_metrics;
    integer total_error_count = 0;
    
    // Tracking "highest" data address written to for
    // displaying only what's needed in the answer key
    // (since there is no need to display the addresses
    // not written to)
    integer max_data_addr;
    
    reg suite_done [NUM_TESTS];
    
    wire [3:0] dmem_data_write;
	`ifdef FEATURE_BIT_ENABLE
		wire [`WORD_WIDTH-1:0] core_data_write;
		assign dmem_data_write = {core_data_write[24], core_data_write[16], core_data_write[8], core_data_write[0]};
	`else
		wire [3:0] core_data_write;
		assign dmem_data_write = core_data_write;
	`endif
    
    wire [`BUS_BITS-1:0] core_data_addr;	
    wire [`DATAMEM_WIDTH-1:0] core_data_store;	
    wire [`DATAMEM_WIDTH-1:0] core_data_load;
    wire core_data_request;
    wire core_data_grant;
    wire core_data_valid;
    
    wire [`PC_ADDR_BITS-1:0] core_inst_addr;
    wire [`WORD_WIDTH-1:0] core_inst_data;
    wire [`WORD_WIDTH-1:0] core_if_inst;
    
    datamem DATAMEM (
        .clk(CLK),
        .nrst(nrst),

        .dm_write(dmem_data_write),
        .data_addr(core_data_addr),        
        .data_in(core_data_store),
        .data_req(core_data_request),
        .data_gnt(core_data_grant),
        .data_valid(core_data_valid),

        .con_write(con_write),
        .con_addr(con_addr),
        .con_in(con_in),
        .con_en(1'b1),

        .data_out(core_data_load),
        .con_out(con_out)
    );
    
    instmem INSTMEM (
        .clk(CLK),
        .nrst(nrst),
        .sel_ISR(1'b0),

        .addr(core_inst_addr),
        .inst(core_inst_data)
    );
    
    core_extmem CORE(
        .clk(CLK),
        .nrst(nrst),

        .int_sig(int_sig),
        
        .ext_data_write(core_data_write),
        .ext_data_addr(core_data_addr),        
        .ext_data_store(core_data_store),
        .ext_data_load(core_data_load),
        .ext_data_req(core_data_request),
        .ext_data_gnt(core_data_grant),
        .ext_data_valid(core_data_valid),
        
        .ext_inst_addr(core_inst_addr),
        .ext_inst_data(core_inst_data),
        .ext_if_inst(core_if_inst)
    );
    
    answerkey_i #() AK();
    
    assign INST = core_if_inst;
        
    assign box = {AK.memory[con_addr][7:0], AK.memory[con_addr][15:8], AK.memory[con_addr][23:16], AK.memory[con_addr][31:24]};
    
    initial begin
        CLK = 0;
    end
    
    genvar i_f;
    integer i_k;
	
	for (i_f = 0; i_f < NUM_TESTS; i_f++) begin
        localparam string temp_data = $sformatf("%s%s%s%s", `REPO_LOCATION, `TEST_LOCATION, "datamem-dump/mem/", file_pile[i_f]);
        localparam string temp_inst = $sformatf("%s%s%s%s", `REPO_LOCATION, `TEST_LOCATION, "instmem-dump/mem/", file_pile[i_f]);
        localparam string temp_refm = $sformatf("%s%s%s%s", `REPO_LOCATION, `TEST_LOCATION, "answer-keys/mem/", file_pile[i_f]);
        
        initial begin
            suite_done[i_f] = 0;
            
            if (i_f != 0) begin
                while(suite_done[i_f-1] == 0) begin
                    #1000;
                end
            end
            
            current_test = test_pile[i_f];
            i_k = i_f;
            nrst = 0;
            CLK = 0;
            last_inst = 0;
            con_write = 0;
            con_addr = 10'h0;
            max_data_addr = 0;
            con_in = 0;
            done = 0;
            check = 0;
            pass = 0;
            print_metrics = 0;
            i = 0;
            j = 0;
            total_test_cases = 0;
            
            $readmemh(temp_data, DATAMEM.COREMEM.ram_block);
            $readmemh(temp_inst, INSTMEM.instmem);
            $readmemh(temp_refm, AK.memory);
            
            #250;
            nrst = 1;
        end
    end 
    // The following code is for checking the contents
    // of BLOCKMEM
    
    always@(posedge CLK) begin
        if(!nrst)
            max_data_addr <= 0;
        else if(!done) 
            if(core_data_request) begin
                if (max_data_addr > 255) begin
                    max_data_addr = 256;
                end
                else begin
                    if (max_data_addr < core_data_addr[`DATAMEM_BITS-1:2])
                        max_data_addr <= core_data_addr[`DATAMEM_BITS-1:2];
                end
            end
    end
    
    always@(posedge done) begin
        
        $display("%s", test_pile[i_k]);
        $display("---------| SUMMARY |---------");
        $display("Address\t  Actual  \tExpected ");
        $display("=======\t==========\t==========");	
    end
    
    always@(negedge CLK) begin
        if(done) begin	
            if(con_out == box) begin
                //$display("0x%3X\t0x%X\t0x%X\tPass", con_addr, con_out, AK.memory[con_addr]);
                pass = pass + 1;
            end else begin
                if (!print_metrics) begin
                    $display("0x%3X\t0x%X\t0x%X\tFail--------------------", con_addr, con_out, box);
                    total_error_count = total_error_count + 1;
                end
            end

            total_test_cases = total_test_cases + 1;
            con_addr = con_addr + 1;
            if(con_addr == max_data_addr) print_metrics = 1;
        end
    end
        
    always@(posedge print_metrics) begin
        $display("Passed %0d/%0d test cases.\n\n", pass, total_test_cases);
        suite_done[i_k] = 1;
        nrst = 0;
        if (i_k == NUM_TESTS - 1) begin
            $display("Error count across %0d tests: %0d. \n\n", NUM_TESTS, total_error_count);
            $finish;
        end
    end 
    
    always@(posedge CLK) begin
        if (!nrst) begin
            check = 0;
            consecutive_nops = 0;
            last_inst = 0;
        end
        else begin
            if (!done) begin
                if ((last_inst[15:0] == 16'h0001 || last_inst== 32'h13) && (INST[15:0] == 16'h0001 || INST== 32'h13)) begin
                    consecutive_nops = consecutive_nops + 1;
                    check = check + 1;
                end
                else if (INST == last_inst) begin
                    check = check + 1;
                end
                else begin
                    last_inst <= INST;
                    consecutive_nops = 0;
                    check = 0;
                end
            end
        end
    end
    
    // This controls the NOP counter
	always@(posedge CLK) begin
	   if (!done)
            if(!nrst)
                nop_counter <= 0;
            else if(!done)
                if(INST[15:0] == 16'h0001 || INST == 32'h00000013)
                    nop_counter <= nop_counter + 1;
	end
	// This controls the done flag
	always@(posedge CLK) begin
		if(check == 50 || consecutive_nops == 8) done = 1;
	end
	
	 
        
endmodule

// ANSWER KEY
module answerkey_i #(parameter REF_OUT = "answerkey.mem")();
	reg [31:0] memory [0:`DATAMEM_DEPTH-1];
	initial begin
	    for (int i = 0; i < `DATAMEM_DEPTH-1; i++) begin
	       memory[i] = 32'd0;
	    end
		$readmemh(REF_OUT, memory);
	end
endmodule