#include<stdint.h>
#include<stdio.h>
#include<string.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// base GF(2^8) mult function
uint8_t gfmul(uint8_t byte, uint8_t mltplr){

    uint32_t res;

    asm volatile("clmul %0, %1, %2"
                : "=r"(res)
                : "r"((uint32_t)byte), "r"((uint32_t)mltplr));
    
    if (res & (1 << 10)) res ^= 0x11B << 2;
    if (res & (1 << 9))  res ^= 0x11B << 1;
    if (res & (1 << 8))  res ^= 0x11B;

    return (uint8_t)res;
    
}

// direct InvMixColumns implementation for use for KeySchedule
void InvMixColumns(block data){

    uint8_t a0 = data[0];
    uint8_t a1 = data[1];
    uint8_t a2 = data[2];
    data[0] = gfmul(a0, 0xE) ^ gfmul(a1, 0xB) ^ gfmul(a2, 0xD) ^ gfmul(data[3], 0x9);
    data[1] = gfmul(a0, 0x9) ^ gfmul(a1, 0xE) ^ gfmul(a2, 0xB) ^ gfmul(data[3], 0xD);
    data[2] = gfmul(a0, 0xD) ^ gfmul(a1, 0x9) ^ gfmul(a2, 0xE) ^ gfmul(data[3], 0xB);
    data[3] = gfmul(a0, 0xB) ^ gfmul(a1, 0xD) ^ gfmul(a2, 0x9) ^ gfmul(data[3], 0xE);

    a0 = data[4];
    a1 = data[5];
    a2 = data[6];
    data[4] = gfmul(a0, 0xE) ^ gfmul(a1, 0xB) ^ gfmul(a2, 0xD) ^ gfmul(data[7], 0x9);
    data[5] = gfmul(a0, 0x9) ^ gfmul(a1, 0xE) ^ gfmul(a2, 0xB) ^ gfmul(data[7], 0xD);
    data[6] = gfmul(a0, 0xD) ^ gfmul(a1, 0x9) ^ gfmul(a2, 0xE) ^ gfmul(data[7], 0xB);
    data[7] = gfmul(a0, 0xB) ^ gfmul(a1, 0xD) ^ gfmul(a2, 0x9) ^ gfmul(data[7], 0xE);

    a0 = data[8];
    a1 = data[9];
    a2 = data[10];
    data[8] = gfmul(a0, 0xE) ^ gfmul(a1, 0xB) ^ gfmul(a2, 0xD) ^ gfmul(data[11], 0x9);
    data[9] = gfmul(a0, 0x9) ^ gfmul(a1, 0xE) ^ gfmul(a2, 0xB) ^ gfmul(data[11], 0xD);
    data[10] = gfmul(a0, 0xD) ^ gfmul(a1, 0x9) ^ gfmul(a2, 0xE) ^ gfmul(data[11], 0xB);
    data[11] = gfmul(a0, 0xB) ^ gfmul(a1, 0xD) ^ gfmul(a2, 0x9) ^ gfmul(data[11], 0xE);

    a0 = data[12];
    a1 = data[13];
    a2 = data[14];
    data[12] = gfmul(a0, 0xE) ^ gfmul(a1, 0xB) ^ gfmul(a2, 0xD) ^ gfmul(data[15], 0x9);
    data[13] = gfmul(a0, 0x9) ^ gfmul(a1, 0xE) ^ gfmul(a2, 0xB) ^ gfmul(data[15], 0xD);
    data[14] = gfmul(a0, 0xD) ^ gfmul(a1, 0x9) ^ gfmul(a2, 0xE) ^ gfmul(data[15], 0xB);
    data[15] = gfmul(a0, 0xB) ^ gfmul(a1, 0xD) ^ gfmul(a2, 0x9) ^ gfmul(data[15], 0xE);

}

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
    
    // InvMixColumns
    InvMixColumns(key_schedule[1]);
    InvMixColumns(key_schedule[2]);
    InvMixColumns(key_schedule[3]);
    InvMixColumns(key_schedule[4]);
    InvMixColumns(key_schedule[5]);
    InvMixColumns(key_schedule[6]);
    InvMixColumns(key_schedule[7]);
    InvMixColumns(key_schedule[8]);
    InvMixColumns(key_schedule[9]);
}

void AES_Decrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t *data_col = (uint32_t*)data;
    uint32_t *key_col = (uint32_t*)key_schedule[0];
    data_col[0] ^= key_col[40];
    data_col[1] ^= key_col[41];
    data_col[2] ^= key_col[42];
    data_col[3] ^= key_col[43];

    // Round 1
    asm volatile("lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "lw a0, 0(%4)\n\t"
                 "lw a1, 0(%5)\n\t"
                 "lw a2, 0(%6)\n\t"
                 "lw a3, 0(%7)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3]), "r"(&key_col[36]), "r"(&key_col[37]), "r"(&key_col[38]), "r"(&key_col[39])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 2
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[32]), "r"(&key_col[33]), "r"(&key_col[34]), "r"(&key_col[35])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 3
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[28]), "r"(&key_col[29]), "r"(&key_col[30]), "r"(&key_col[31])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 4
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[24]), "r"(&key_col[25]), "r"(&key_col[26]), "r"(&key_col[27])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 5
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[20]), "r"(&key_col[21]), "r"(&key_col[22]), "r"(&key_col[23])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 6
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[16]), "r"(&key_col[17]), "r"(&key_col[18]), "r"(&key_col[19])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 7
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[12]), "r"(&key_col[13]), "r"(&key_col[14]), "r"(&key_col[15])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 8
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[8]), "r"(&key_col[9]), "r"(&key_col[10]), "r"(&key_col[11])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 9
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsmi a0, a0, t0, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, a1, t1, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, a2, t2, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi t0, a0, t3, 1\n\t"
                 "aes32dsmi t1, a1, t3, 2\n\t"
                 "aes32dsmi t2, a2, t3, 3\n\t"
                 "aes32dsmi t3, a3, t3, 0\n\t"
                :
                : "r"(&key_col[4]), "r"(&key_col[5]), "r"(&key_col[6]), "r"(&key_col[7])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 10
    asm volatile("lw a0, 0(%0)\n\t"
                 "lw a1, 0(%1)\n\t"
                 "lw a2, 0(%2)\n\t"
                 "lw a3, 0(%3)\n\t"
                 "aes32dsi a0, a0, t0, 0\n\t"
                 "aes32dsi a1, a1, t0, 1\n\t"
                 "aes32dsi a2, a2, t0, 2\n\t"
                 "aes32dsi a3, a3, t0, 3\n\t"
                 "aes32dsi a0, a0, t1, 3\n\t"
                 "aes32dsi a1, a1, t1, 0\n\t"
                 "aes32dsi a2, a2, t1, 1\n\t"
                 "aes32dsi a3, a3, t1, 2\n\t"
                 "aes32dsi a0, a0, t2, 2\n\t"
                 "aes32dsi a1, a1, t2, 3\n\t"
                 "aes32dsi a2, a2, t2, 0\n\t"
                 "aes32dsi a3, a3, t2, 1\n\t"
                 "aes32dsi a0, a0, t3, 1\n\t"
                 "aes32dsi a1, a1, t3, 2\n\t"
                 "aes32dsi a2, a2, t3, 3\n\t"
                 "aes32dsi a3, a3, t3, 0\n\t"
                 "sw a0, 0(%4)\n\t"
                 "sw a1, 0(%5)\n\t"
                 "sw a2, 0(%6)\n\t"
                 "sw a3, 0(%7)\n\t"
                :
                : "r"(&key_col[0]), "r"(&key_col[1]), "r"(&key_col[2]), "r"(&key_col[3]), "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3", "memory");

}

// Main CBC mode function
// Inputs: ciphertext array, plaintext empty array, number of blocks of the ciphertext, input key, key schedule array, initialization vector
// Output: plaintext array
void CBC(block ciphertext[], block plaintext[], size_t num_blocks, block key_schedule[11], block IV){

    // Perform KeySchedule to generate round keys
    KeySchedule(key_schedule);

    // copy first ciphertext block contents to first plaintext block
    uint32_t* ciphertext_col = (uint32_t*)ciphertext[0];
    uint32_t* plaintext_col = (uint32_t*)plaintext[0];
    plaintext_col[0] = ciphertext_col[0];
    plaintext_col[1] = ciphertext_col[1];
    plaintext_col[2] = ciphertext_col[2];
    plaintext_col[3] = ciphertext_col[3];

    // handle first block
    // perform AES Decrypt
    AES_Decrypt(plaintext[0], key_schedule);
    // XOR with IV
    uint32_t* IV_col = (uint32_t*)IV;
    plaintext_col[0] = plaintext_col[0] ^ IV_col[0];
    plaintext_col[1] = plaintext_col[1] ^ IV_col[1];
    plaintext_col[2] = plaintext_col[2] ^ IV_col[2];
    plaintext_col[3] = plaintext_col[3] ^ IV_col[3];

    for(size_t i = 1; i < num_blocks; i++){
        uint32_t idx = i << 2; // i * 4
        uint32_t prev_idx = (i - 1) << 2; // (i - 1) * 4
        // copy ciphertext to plaintext
        plaintext_col[idx] = ciphertext_col[idx];
        plaintext_col[idx + 1] = ciphertext_col[idx + 1];
        plaintext_col[idx + 2] = ciphertext_col[idx + 2];
        plaintext_col[idx + 3] = ciphertext_col[idx + 3];
        // perform AES Decrypt
        AES_Decrypt(plaintext[i], key_schedule);
        // XOR with previous ciphertext
        plaintext_col[idx] = plaintext_col[idx] ^ ciphertext_col[prev_idx];
        plaintext_col[idx + 1] = plaintext_col[idx + 1] ^ ciphertext_col[prev_idx + 1];
        plaintext_col[idx + 2] = plaintext_col[idx + 2] ^ ciphertext_col[prev_idx + 2];
        plaintext_col[idx + 3] = plaintext_col[idx + 3] ^ ciphertext_col[prev_idx + 3];
    }
}

// Global input
block in_ciphertext[10] = {
    {
        0x61, 0xc3, 0x13, 0x2a, 
        0x77, 0x7e, 0x4e, 0x96, 
        0x8a, 0xa7, 0x07, 0x44, 
        0x73, 0xfd, 0x35, 0x61
    },
    {
        0x3b, 0xc7, 0xe2, 0x4d, 
        0xc1, 0x04, 0xf4, 0x50, 
        0x27, 0xb7, 0x43, 0x81, 
        0x34, 0x81, 0xb4, 0x75
    },
    {
        0x27, 0x71, 0xe0, 0x0e, 
        0x10, 0xc1, 0x5a, 0x88, 
        0xc8, 0x96, 0x58, 0xd0, 
        0x55, 0xd7, 0xdb, 0x24
    },
    {
        0xfa, 0x71, 0x6c, 0x73, 
        0x26, 0x80, 0xad, 0x28, 
        0x88, 0xf7, 0xdc, 0x3c, 
        0xc2, 0xea, 0x4c, 0x2e
    },
    {
        0xa6, 0xee, 0x4d, 0xf0, 
        0x57, 0xcd, 0xc1, 0x6d, 
        0x76, 0x86, 0x47, 0x7c, 
        0x00, 0xf2, 0x1d, 0x1a
    },
    {
        0xcf, 0x6e, 0xed, 0x42, 
        0x2d, 0xda, 0x05, 0xb4, 
        0x81, 0xa2, 0xe4, 0x00, 
        0xc0, 0x6c, 0x75, 0xc0
    },
    {
        0x30, 0x84, 0xd5, 0x6b, 
        0xee, 0x83, 0x42, 0x9e, 
        0xae, 0x7d, 0xe1, 0x5b, 
        0xa1, 0x86, 0x5b, 0xb7
    },
    {
        0xab, 0x51, 0x38, 0xcd, 
        0xa7, 0x13, 0x6e, 0x21, 
        0xdc, 0x70, 0x9a, 0x7c, 
        0xa2, 0x9a, 0xcc, 0x13
    },
    {
        0xbe, 0x61, 0x27, 0xb3, 
        0xcd, 0xb2, 0x31, 0xdc, 
        0xad, 0xa7, 0x4b, 0x97, 
        0x86, 0x41, 0xf2, 0x6e
    },
    {
        0x5a, 0xf8, 0x77, 0x80, 
        0x4e, 0x46, 0x07, 0x97, 
        0x69, 0x3d, 0xbd, 0x27, 
        0xfd, 0x62, 0x3f, 0xa0
    }
};

// Global output
block out_plaintext[10];

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
    CBC(in_ciphertext, out_plaintext, 10, in_key, init_vector);
}