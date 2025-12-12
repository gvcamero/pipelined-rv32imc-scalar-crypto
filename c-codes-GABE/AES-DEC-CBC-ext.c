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
block in_ciphertext[50] = {
    {
        0x76, 0x49, 0xab, 0xac, 
        0x81, 0x19, 0xb2, 0x46, 
        0xce, 0xe9, 0x8e, 0x9b, 
        0x12, 0xe9, 0x19, 0x7d
    },
    {
        0x50, 0x86, 0xcb, 0x9b, 
        0x50, 0x72, 0x19, 0xee, 
        0x95, 0xdb, 0x11, 0x3a, 
        0x91, 0x76, 0x78, 0xb2
    },
    {
        0x73, 0xbe, 0xd6, 0xb8, 
        0xe3, 0xc1, 0x74, 0x3b, 
        0x71, 0x16, 0xe6, 0x9e, 
        0x22, 0x22, 0x95, 0x16
    },
    {
        0x3f, 0xf1, 0xca, 0xa1, 
        0x68, 0x1f, 0xac, 0x09, 
        0x12, 0x0e, 0xca, 0x30, 
        0x75, 0x86, 0xe1, 0xa7
    },
    {
        0xd7, 0x35, 0x49, 0xfa, 
        0xc8, 0x02, 0x8b, 0x8b, 
        0x5f, 0x40, 0xbd, 0xc6, 
        0x84, 0xc0, 0xf0, 0xc2
    },
    {
        0x09, 0x0e, 0xd1, 0x61, 
        0xeb, 0xd7, 0x58, 0x05, 
        0xf0, 0x80, 0x5d, 0xc3, 
        0xc4, 0xaa, 0x17, 0x56
    },
    {
        0x6d, 0xf8, 0x73, 0xc0, 
        0x7c, 0xc9, 0x07, 0x06, 
        0x67, 0x11, 0x6b, 0x80, 
        0x64, 0xca, 0x7d, 0x31
    },
    {
        0x01, 0x71, 0x7e, 0x33, 
        0xb2, 0xe2, 0xb0, 0xa4, 
        0x14, 0x06, 0x27, 0xe3, 
        0x23, 0x14, 0x34, 0x97
    },
    {
        0x2e, 0x62, 0x21, 0xce, 
        0xfe, 0x67, 0xd6, 0x28, 
        0x8f, 0x1f, 0x75, 0xd6, 
        0x9a, 0xf0, 0x40, 0x75
    },
    {
        0xda, 0x6d, 0xa3, 0xbc, 
        0xc6, 0x39, 0x48, 0x84, 
        0xab, 0x36, 0xf4, 0x06, 
        0xca, 0x11, 0x3e, 0x8a
    },
    // BATCH 2
    {
        0x57, 0xe9, 0xf6, 0x51, 
        0xad, 0xd3, 0xdb, 0xdb, 
        0xd6, 0x30, 0xb6, 0x95, 
        0x0f, 0x06, 0xef, 0xed
    },
    {
        0x86, 0x77, 0x31, 0x00, 
        0xb9, 0xcd, 0x69, 0xf4, 
        0xf1, 0x09, 0xe1, 0xda, 
        0x92, 0x75, 0x09, 0x4c
    },
    {
        0x6e, 0x7e, 0x78, 0xc3, 
        0x4f, 0x61, 0x5b, 0xfe, 
        0xbd, 0x86, 0x47, 0x32, 
        0x5c, 0x38, 0x9c, 0x7a
    },
    {
        0xad, 0xd2, 0xdf, 0x21, 
        0x95, 0x64, 0x37, 0x06, 
        0x20, 0x3e, 0x93, 0x72, 
        0x4d, 0x2f, 0x9a, 0xc5
    },
    {
        0xa1, 0x9d, 0x9f, 0x0a, 
        0xee, 0xfa, 0x75, 0x8c, 
        0xfe, 0x9d, 0x55, 0x6f, 
        0xe4, 0x44, 0xcd, 0x14
    },
    {
        0x86, 0xc2, 0xb0, 0xb6, 
        0x25, 0xbf, 0x22, 0x70, 
        0x5d, 0x03, 0xa9, 0xde, 
        0x2d, 0x25, 0xf0, 0x59
    },
    {
        0x2b, 0x48, 0x37, 0x47, 
        0x9f, 0xef, 0x0b, 0xeb, 
        0xd5, 0x25, 0xbd, 0x19, 
        0x94, 0x28, 0x52, 0xed
    },
    {
        0x7e, 0x7c, 0x9a, 0x48, 
        0xed, 0x4a, 0xe4, 0x50, 
        0xad, 0x8c, 0x17, 0xca, 
        0x6f, 0xf0, 0x2a, 0xb5
    },
    {
        0x38, 0x5d, 0xa4, 0xd4, 
        0x71, 0xf8, 0x9b, 0x30, 
        0x98, 0xbe, 0x48, 0x9c, 
        0x68, 0xdb, 0x8e, 0xe2
    },
    {
        0x02, 0x4e, 0x01, 0xc2, 
        0x20, 0xf5, 0xba, 0x57, 
        0x19, 0xfc, 0xf0, 0x66, 
        0x6e, 0xed, 0x25, 0xc3
    },
    // BATCH 3
    {
        0x52, 0xb0, 0x61, 0x78, 
        0x60, 0xb4, 0x45, 0xba, 
        0x0c, 0xa6, 0x36, 0x43, 
        0x96, 0xcf, 0xc2, 0x8c
    },
    {
        0xb7, 0xc9, 0xb3, 0xde, 
        0x57, 0xcc, 0x35, 0xc1, 
        0x97, 0x89, 0x5b, 0x6e, 
        0x6e, 0x9b, 0xd8, 0xc9
    },
    {
        0x3c, 0xfe, 0xcb, 0x85, 
        0x34, 0x50, 0x19, 0x06, 
        0x60, 0x8e, 0x4a, 0xb6, 
        0x1e, 0x73, 0x6a, 0xeb
    },
    {
        0xcc, 0x3b, 0xc0, 0x09, 
        0xb1, 0x15, 0xc5, 0xb1, 
        0xdf, 0xe5, 0x13, 0x9c, 
        0x62, 0x8c, 0x0f, 0xa1
    },
    {
        0x75, 0xb1, 0x4c, 0x16, 
        0xb7, 0x6c, 0x00, 0x3a, 
        0x32, 0x5c, 0x6b, 0x93, 
        0x58, 0x1a, 0x21, 0xe4
    },
    {
        0x2e, 0x22, 0xe2, 0x0e, 
        0x84, 0x86, 0xa1, 0x7c, 
        0x88, 0x56, 0xb3, 0x4e, 
        0xc7, 0xc3, 0xa0, 0xc0
    },
    {
        0x10, 0x5b, 0xde, 0xe3, 
        0xec, 0xea, 0xc0, 0x54, 
        0xf7, 0x02, 0x0b, 0xc1, 
        0x24, 0x8f, 0xd8, 0x27
    },
    {
        0x19, 0x9d, 0xb1, 0x17, 
        0x74, 0xb5, 0x54, 0x3e, 
        0xfa, 0x00, 0xe5, 0x0d, 
        0xa9, 0xe1, 0xf7, 0x23
    },
    {
        0x43, 0x00, 0xa6, 0x45, 
        0x56, 0x36, 0xd8, 0x8d, 
        0xa3, 0xf2, 0xc9, 0xa1, 
        0xf5, 0x02, 0xf4, 0x7c
    },
    {
        0x71, 0x82, 0x5c, 0x1e, 
        0x68, 0x2a, 0xa9, 0x9d, 
        0x2b, 0x27, 0x48, 0x5d, 
        0xe3, 0xd6, 0x02, 0x88
    },
    // BATCH 4
    {
        0x36, 0x0e, 0x7a, 0x80, 
        0x06, 0x5e, 0x7b, 0x1f, 
        0xdd, 0xed, 0x00, 0x12, 
        0x8f, 0x1c, 0xa0, 0x3c
    },
    {
        0x07, 0x3a, 0x6d, 0xd5, 
        0x10, 0x6d, 0x25, 0x7d, 
        0xe0, 0x14, 0x11, 0xa4, 
        0xb3, 0xef, 0xa4, 0x1a
    },
    {
        0x04, 0x87, 0xfa, 0x1b, 
        0xd6, 0x0b, 0xba, 0x37, 
        0x5c, 0x6b, 0xa6, 0x88, 
        0xca, 0x6f, 0x38, 0xa3
    },
    {
        0x4f, 0x99, 0x46, 0x06, 
        0x4c, 0x5f, 0x7f, 0xe8, 
        0xab, 0x50, 0xc3, 0x76, 
        0x7b, 0xad, 0xc5, 0xad
    },
    {
        0x8e, 0x5b, 0x33, 0x73, 
        0x08, 0x49, 0x2d, 0x4f, 
        0xa4, 0x0a, 0xeb, 0x81, 
        0xf0, 0xa3, 0x66, 0x7c
    },
    {
        0xfe, 0x6d, 0xc9, 0x69, 
        0xce, 0x75, 0x88, 0xfd, 
        0xb2, 0x28, 0x2f, 0x29, 
        0x8c, 0x03, 0x8c, 0x28
    },
    {
        0xb5, 0xb1, 0xdd, 0x98, 
        0x77, 0xfb, 0xf8, 0x4e, 
        0xeb, 0x78, 0x8f, 0x69, 
        0x5a, 0x6d, 0x81, 0x69
    },
    {
        0x5d, 0x66, 0xd7, 0x8f, 
        0x88, 0x5a, 0xe5, 0xd3, 
        0xb2, 0x0b, 0xb8, 0xf7, 
        0xb9, 0xfe, 0x13, 0xf7
    },
    {
        0xc8, 0x27, 0x50, 0x43, 
        0x25, 0xf5, 0x60, 0xbe, 
        0xea, 0x08, 0x11, 0xd7, 
        0x13, 0x6c, 0x40, 0xc2
    },
    {
        0x37, 0xa3, 0x41, 0x89, 
        0x6d, 0x38, 0xbd, 0xc3, 
        0x58, 0x8e, 0xb1, 0x4d, 
        0xa1, 0xbb, 0x3e, 0x3c
    },
    // BATCH 5
    {
        0xe1, 0xad, 0xd5, 0xb6, 
        0x13, 0x44, 0x9b, 0x6d, 
        0x30, 0xf6, 0xc7, 0x9e, 
        0x90, 0x7a, 0x00, 0xa5
    },
    {
        0xea, 0xf1, 0xb6, 0x51, 
        0xbd, 0xbe, 0xe6, 0x26, 
        0xa1, 0xb8, 0xf2, 0x6e, 
        0xb4, 0xd0, 0xa8, 0x03
    },
    {
        0xa6, 0x56, 0x12, 0xf0, 
        0xbd, 0xd5, 0x36, 0xc5, 
        0xd8, 0xb6, 0x14, 0x11, 
        0x87, 0x2a, 0x71, 0xcd
    },
    {
        0xa7, 0xbf, 0x95, 0x42, 
        0x7d, 0x63, 0x1a, 0x68, 
        0xdb, 0x7e, 0x46, 0x34, 
        0xd9, 0xe5, 0x3d, 0x9f
    },
    {
        0x5a, 0x4d, 0xb1, 0x44, 
        0xfc, 0x6d, 0xb2, 0x5f, 
        0xc1, 0xa1, 0x4a, 0xef, 
        0x41, 0xf5, 0xa4, 0xdf
    },
    {
        0xef, 0x4d, 0x32, 0xcc, 
        0x08, 0xe8, 0x0e, 0xa5, 
        0xad, 0xc1, 0xf3, 0x63, 
        0xeb, 0x58, 0x9b, 0x60
    },
    {
        0x64, 0x43, 0xbd, 0xbb, 
        0x24, 0x3a, 0x78, 0x6a, 
        0xd2, 0x51, 0x9e, 0x10, 
        0xa2, 0x7c, 0x30, 0x26
    },
    {
        0xbc, 0x27, 0xa1, 0x55, 
        0xd2, 0x81, 0xb5, 0x10, 
        0x70, 0x62, 0xb9, 0x78, 
        0xf8, 0x64, 0xdf, 0x58
    },
    {
        0xff, 0x6f, 0x89, 0xa2, 
        0x1e, 0xdc, 0xc3, 0x39, 
        0x95, 0x2d, 0xa0, 0x95, 
        0x87, 0x4b, 0x05, 0x1d
    },
    {
        0x8e, 0xa1, 0x9a, 0x91, 
        0x18, 0x0b, 0xcf, 0x25, 
        0xd6, 0xc8, 0xcf, 0x2a, 
        0xdb, 0x2a, 0x46, 0xcf
    }
};

// Global output
block out_plaintext[50];

// Global key
block in_key[11] = {
    {
        0x2b, 0x7e, 0x15, 0x16, 
        0x28, 0xae, 0xd2, 0xa6, 
        0xab, 0xf7, 0x15, 0x88, 
        0x09, 0xcf, 0x4f, 0x3c
    }
};

// Initialization vector
block init_vector = {
    0x00, 0x01, 0x02, 0x03, 
    0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0a, 0x0b, 
    0x0c, 0x0d, 0x0e, 0x0f
};

int main(){
    CBC(in_ciphertext, out_plaintext, 50, in_key, init_vector);
}