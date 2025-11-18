#include<stdint.h>
#include<stdio.h>
#include<string.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// KeySchedule function
// Inputs: pointer to array of 11 keys, (first block: input key; the rest are empty)
// Output: entire key schedule in the rest of the array
void KeySchedule(block key_schedule[11]){

    uint32_t* key_sched_col = (uint32_t*)key_schedule[0];

    /* Sequence
    %0 = value of previous key, fourth column
    %1 = value of previous key, first column
    %2 = address of new key, first column
    %3 = value of previous key, second column
    %4 = address of new key, second column
    %5 = value of previous key, third column
    %6 = address of new key, third column
    %7 = addres of new key, fourth column*/
    // Key 1
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x01\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[3]), "r"(key_sched_col[0]), "r"(&key_sched_col[4]), "r"(key_sched_col[1]), "r"(&key_sched_col[5]), "r"(key_sched_col[2]), "r"(&key_sched_col[6]), "r"(&key_sched_col[7])
                : "t1", "t0", "memory");
    
    // Key 2
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x02\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[7]), "r"(key_sched_col[4]), "r"(&key_sched_col[8]), "r"(key_sched_col[5]), "r"(&key_sched_col[9]), "r"(key_sched_col[6]), "r"(&key_sched_col[10]), "r"(&key_sched_col[11])
                : "t1", "t0", "memory");

    // Key 3
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x04\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[11]), "r"(key_sched_col[8]), "r"(&key_sched_col[12]), "r"(key_sched_col[9]), "r"(&key_sched_col[13]), "r"(key_sched_col[10]), "r"(&key_sched_col[14]), "r"(&key_sched_col[15])
                : "t1", "t0", "memory");

    // Key 4
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x08\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[15]), "r"(key_sched_col[12]), "r"(&key_sched_col[16]), "r"(key_sched_col[13]), "r"(&key_sched_col[17]), "r"(key_sched_col[14]), "r"(&key_sched_col[18]), "r"(&key_sched_col[19])
                : "t1", "t0", "memory");

    // Key 5
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x10\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[19]), "r"(key_sched_col[16]), "r"(&key_sched_col[20]), "r"(key_sched_col[17]), "r"(&key_sched_col[21]), "r"(key_sched_col[18]), "r"(&key_sched_col[22]), "r"(&key_sched_col[23])
                : "t1", "t0", "memory");

    // Key 6
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x20\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[23]), "r"(key_sched_col[20]), "r"(&key_sched_col[24]), "r"(key_sched_col[21]), "r"(&key_sched_col[25]), "r"(key_sched_col[22]), "r"(&key_sched_col[26]), "r"(&key_sched_col[27])
                : "t1", "t0", "memory");

    // Key 7
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x40\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[27]), "r"(key_sched_col[24]), "r"(&key_sched_col[28]), "r"(key_sched_col[25]), "r"(&key_sched_col[29]), "r"(key_sched_col[26]), "r"(&key_sched_col[30]), "r"(&key_sched_col[31])
                : "t1", "t0", "memory");

    // Key 8
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x80\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[31]), "r"(key_sched_col[28]), "r"(&key_sched_col[32]), "r"(key_sched_col[29]), "r"(&key_sched_col[33]), "r"(key_sched_col[30]), "r"(&key_sched_col[34]), "r"(&key_sched_col[35])
                : "t1", "t0", "memory");

    // Key 9
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x1B\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[35]), "r"(key_sched_col[32]), "r"(&key_sched_col[36]), "r"(key_sched_col[33]), "r"(&key_sched_col[37]), "r"(key_sched_col[34]), "r"(&key_sched_col[38]), "r"(&key_sched_col[39])
                : "t1", "t0", "memory");

    // Key 10
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, %1, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xori t0, t0, 0x36\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[39]), "r"(key_sched_col[36]), "r"(&key_sched_col[40]), "r"(key_sched_col[37]), "r"(&key_sched_col[41]), "r"(key_sched_col[38]), "r"(&key_sched_col[42]), "r"(&key_sched_col[43])
                : "t1", "t0", "memory");
    
}

// AES Encrypt function
// Inputs: block to be encrypted, array of input key + key schedule
// Output: encrypted block
void AES_Encrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t *data_col = (uint32_t*)data;
    uint32_t *key_col = (uint32_t*)key_schedule[0];
    data_col[0] ^= key_col[0];
    data_col[1] ^= key_col[1];
    data_col[2] ^= key_col[2];
    data_col[3] ^= key_col[3];

    /*Sequence
    %0 = address of data 1st column
    %1 = address of data 2nd column
    %2 = address of data 3rd column
    %3 = address of data 4th column
    %4 = address of key 1st column
    %5 = address of key 2nd column
    %6 = address of key 3rd column
    %7 = address of key 4th column*/
    // Round 1
    asm volatile("lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "lw a0, 0(%4)\n\t"
                 "lw a1, 0(%5)\n\t"
                 "lw a2, 0(%6)\n\t"
                 "lw a3, 0(%7)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3]), "r"(&key_col[4]), "r"(&key_col[5]), "r"(&key_col[6]), "r"(&key_col[7])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 2
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[8]), "r"(&key_col[9]), "r"(&key_col[10]), "r"(&key_col[11])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 3
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[12]), "r"(&key_col[13]), "r"(&key_col[14]), "r"(&key_col[15])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 4
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[16]), "r"(&key_col[17]), "r"(&key_col[18]), "r"(&key_col[19])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 5
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[20]), "r"(&key_col[21]), "r"(&key_col[22]), "r"(&key_col[23])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 6
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[24]), "r"(&key_col[25]), "r"(&key_col[26]), "r"(&key_col[27])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 7
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[28]), "r"(&key_col[29]), "r"(&key_col[30]), "r"(&key_col[31])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 8
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[32]), "r"(&key_col[33]), "r"(&key_col[34]), "r"(&key_col[35])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 9
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esmi a0, a0, t0, 0\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a1, a1, t1, 0\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a2, a2, t2, 0\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "aes32esmi t0, a0, t3, 3\n\t"
                 "aes32esmi t1, a1, t3, 2\n\t"
                 "aes32esmi t2, a2, t3, 1\n\t"
                 "aes32esmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[36]), "r"(&key_col[37]), "r"(&key_col[38]), "r"(&key_col[39])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 10
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32esi a0, a0, t0, 0\n\t"
                 "aes32esi a1, a1, t0, 3\n\t"
                 "aes32esi a2, a2, t0, 2\n\t"
                 "aes32esi a3, a3, t0, 1\n\t"
                 "aes32esi a0, a0, t1, 1\n\t"
                 "aes32esi a1, a1, t1, 0\n\t"
                 "aes32esi a2, a2, t1, 3\n\t"
                 "aes32esi a3, a3, t1, 2\n\t"
                 "aes32esi a0, a0, t2, 2\n\t"
                 "aes32esi a1, a1, t2, 1\n\t"
                 "aes32esi a2, a2, t2, 0\n\t"
                 "aes32esi a3, a3, t2, 3\n\t"
                 "aes32esi a0, a0, t3, 3\n\t"
                 "aes32esi a1, a1, t3, 2\n\t"
                 "aes32esi a2, a2, t3, 1\n\t"
                 "aes32esi a3, a3, t3, 0\n\t"
                 "sw a0, 0(%4)\n\t"
                 "sw a1, 0(%5)\n\t"
                 "sw a2, 0(%6)\n\t"
                 "sw a3, 0(%7)\n\t"
                :
                : "r"(&key_col[40]), "r"(&key_col[41]), "r"(&key_col[42]), "r"(&key_col[43]), "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3", "memory");

}

// Main CBC mode function
// Inputs: plaintext array, number of blocks of the plaintext, input key, key schedule array, initialization vector
// Output: ciphertext array (same array as plaintext)
void CBC(block plaintext[], size_t num_blocks, block key_schedule[11], block IV){

    // Perform KeySchedule to generate round keys
    KeySchedule(key_schedule);

    // handle first block
    uint32_t* plaintext_col = (uint32_t*)plaintext[0];
    uint32_t* IV_col = (uint32_t*)IV;
    // XOR plaintext and IV
    plaintext_col[0] = plaintext_col[0] ^ IV_col[0];
    plaintext_col[1] = plaintext_col[1] ^ IV_col[1];
    plaintext_col[2] = plaintext_col[2] ^ IV_col[2];
    plaintext_col[3] = plaintext_col[3] ^ IV_col[3];
    // Perform AES Encrypt
    AES_Encrypt(plaintext[0], key_schedule); // output ciphertext 0

    for(size_t i = 1; i < num_blocks; i++){
        // XOR previous ciphertext with next plaintext
        uint32_t idx = i << 2; // i * 4
        uint32_t prev_idx = (i - 1) << 2; // (i - 1) * 4
        plaintext_col[idx] = plaintext_col[idx] ^ plaintext_col[prev_idx];
        plaintext_col[idx + 1] = plaintext_col[idx + 1] ^ plaintext_col[prev_idx + 1];
        plaintext_col[idx + 2] = plaintext_col[idx + 2] ^ plaintext_col[prev_idx + 2];
        plaintext_col[idx + 3] = plaintext_col[idx + 3] ^ plaintext_col[prev_idx + 3];
        // Encrypt result
        AES_Encrypt(plaintext[i], key_schedule); // output ciphertext i
    }
}

// Global input
block in_plaintext[10] = {
    {
        0xf9, 0xb7, 0x92, 0xa1, 
        0x19, 0xc5, 0x84, 0xa6, 
        0x0c, 0x54, 0x2b, 0xe9, 
        0xa5, 0xe1, 0xfd, 0xcd
    },
    {
        0x46, 0x10, 0xd7, 0x3b, 
        0x78, 0x44, 0x66, 0x2d, 
        0xdc, 0xf3, 0xf3, 0x69, 
        0xc6, 0xc7, 0xbd, 0x8e
    },
    {
        0xd6, 0x3c, 0xda, 0x83, 
        0xe1, 0x35, 0x96, 0x61, 
        0x8e, 0x6c, 0x14, 0x5b, 
        0xc9, 0x4d, 0x88, 0x9a
    },
    {
        0xc3, 0x71, 0xec, 0xfb, 
        0xd5, 0xa9, 0xca, 0x24, 
        0xe5, 0xa8, 0x2a, 0x29, 
        0xc7, 0xc8, 0x3b, 0xff
    },
    {
        0x03, 0xce, 0xc9, 0x88, 
        0x5d, 0xad, 0xdd, 0x4e, 
        0x8d, 0xd9, 0xaf, 0x55, 
        0xd0, 0x0c, 0x73, 0x05
    },
    {
        0x99, 0x58, 0x8f, 0x89, 
        0x59, 0x20, 0x76, 0xba, 
        0x08, 0x54, 0x3c, 0xe9, 
        0xf4, 0x23, 0xe0, 0x19
    },
    {
        0x6b, 0xc2, 0x5a, 0x91, 
        0x05, 0x6f, 0x5c, 0xed, 
        0x2a, 0xc7, 0x99, 0x48, 
        0xb5, 0xa2, 0xf6, 0xb7
    },
    {
        0x91, 0x75, 0x24, 0x0b, 
        0x8c, 0xa0, 0xf7, 0x99, 
        0x96, 0x13, 0x98, 0x7c, 
        0x72, 0xdc, 0xd6, 0xc0
    },
    {
        0x90, 0x96, 0xba, 0x56, 
        0x2d, 0xdc, 0xcc, 0xba, 
        0x1e, 0xa7, 0x9a, 0x5a, 
        0x04, 0x3c, 0x9a, 0x9a
    },
    {
        0xa2, 0x61, 0x32, 0x5f, 
        0x49, 0xab, 0xf2, 0xea, 
        0x9b, 0xed, 0x27, 0xd9, 
        0x7b, 0x8b, 0xbd, 0xc7
    }
};

// Global key
block in_key[11] = {
    {
        0xa9, 0x0a, 0x8f, 0x86, 
        0xf0, 0x56, 0x5b, 0xf8, 
        0x60, 0x1e, 0x43, 0x9d, 
        0xaf, 0x94, 0x86, 0x7a
    }
};

// Initialization vector
block init_vector = {
    0x89, 0x25, 0xdd, 0x48, 
    0x2c, 0x05, 0x30, 0x7c, 
    0x7b, 0x35, 0x46, 0x8a, 
    0x8d, 0xa3, 0x8b, 0x1a
};

int main(){
    CBC(in_plaintext, 10, in_key, init_vector);
}