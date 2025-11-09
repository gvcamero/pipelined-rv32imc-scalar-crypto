/*
AES-128 Decryption Program with Extensions by Gabriel Camero
To be verified
*/

#include<stdint.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// Global variables
block key_schedule[10]; // Round keys array

// InvMixColumns

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

void InvMixColumns(block data){

    for(int i = 0; i < 16; i += 4){

        uint8_t a0 = data[i];
        uint8_t a1 = data[i+1];
        uint8_t a2 = data[i+2];

        data[i] = gfmul(a0, 0xE) ^ gfmul(a1, 0xB) ^ gfmul(a2, 0xD) ^ gfmul(data[i+3], 0x9);
        data[i+1] = gfmul(a0, 0x9) ^ gfmul(a1, 0xE) ^ gfmul(a2, 0xB) ^ gfmul(data[i+3], 0xD);
        data[i+2] = gfmul(a0, 0xD) ^ gfmul(a1, 0x9) ^ gfmul(a2, 0xE) ^ gfmul(data[i+3], 0xB);
        data[i+3] = gfmul(a0, 0xB) ^ gfmul(a1, 0xD) ^ gfmul(a2, 0x9) ^ gfmul(data[i+3], 0xE);
    }

}

// Key Schedule
void KeySchedule(block key){

    // UNROLLED VERSION
    
    // key_schedule[0]
    uint32_t *old_key_col = (uint32_t*)&key[0];
    uint32_t *new_key_col = (uint32_t*)&key_schedule[0][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x01\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");
    
    // key_schedule[1]
    old_key_col = (uint32_t*)&key_schedule[0][0];
    new_key_col = (uint32_t*)&key_schedule[1][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x02\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[2]
    old_key_col = (uint32_t*)&key_schedule[1][0];
    new_key_col = (uint32_t*)&key_schedule[2][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x04\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[3]
    old_key_col = (uint32_t*)&key_schedule[2][0];
    new_key_col = (uint32_t*)&key_schedule[3][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x08\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[4]
    old_key_col = (uint32_t*)&key_schedule[3][0];
    new_key_col = (uint32_t*)&key_schedule[4][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x10\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[5]
    old_key_col = (uint32_t*)&key_schedule[4][0];
    new_key_col = (uint32_t*)&key_schedule[5][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x20\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[6]
    old_key_col = (uint32_t*)&key_schedule[5][0];
    new_key_col = (uint32_t*)&key_schedule[6][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x40\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[7]
    old_key_col = (uint32_t*)&key_schedule[6][0];
    new_key_col = (uint32_t*)&key_schedule[7][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x80\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[8]
    old_key_col = (uint32_t*)&key_schedule[7][0];
    new_key_col = (uint32_t*)&key_schedule[8][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x1B\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");

    // key_schedule[9]
    old_key_col = (uint32_t*)&key_schedule[8][0];
    new_key_col = (uint32_t*)&key_schedule[9][0];
    // RotWord and SubBytes
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x36\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3])
                : "t1", "t0", "memory");
    
    // InvMixColumns
    for(int i = 0; i < 9; i++){
        InvMixColumns(key_schedule[i]);
    }

}

void aes_dec(block data, block key){
    
    // KeySchedule
    KeySchedule(key);

    // Initial AddRoundKey
    for(int i = 0; i < 16; i++){
        data[i] ^= key_schedule[9][i];
    }
    
    uint32_t *data_col = (uint32_t*)&data[0];
    uint32_t *key_col = (uint32_t*)&key_schedule[0][0];

    // Round 1
    asm volatile("lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%4)\n\t"
                 "lw t1, 0(%5)\n\t"
                 "lw t2, 0(%6)\n\t"
                 "lw t3, 0(%7)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3]), "r"(&key_col[32]), "r"(&key_col[33]), "r"(&key_col[34]), "r"(&key_col[35])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");
    
    // Round 2
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[28]), "r"(&key_col[29]), "r"(&key_col[30]), "r"(&key_col[31])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 3
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[24]), "r"(&key_col[25]), "r"(&key_col[26]), "r"(&key_col[27])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 4
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[20]), "r"(&key_col[21]), "r"(&key_col[22]), "r"(&key_col[23])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 5
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[16]), "r"(&key_col[17]), "r"(&key_col[18]), "r"(&key_col[19])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 6
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[12]), "r"(&key_col[13]), "r"(&key_col[14]), "r"(&key_col[15])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 7
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[8]), "r"(&key_col[9]), "r"(&key_col[10]), "r"(&key_col[11])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 8
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[4]), "r"(&key_col[5]), "r"(&key_col[6]), "r"(&key_col[7])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 9
    asm volatile("aes32dsmi a0, zero, t0, 0\n\t"
                 "aes32dsmi a0, a0, t3, 1\n\t"
                 "aes32dsmi a0, a0, t2, 2\n\t"
                 "aes32dsmi a0, a0, t1, 3\n\t"
                 "aes32dsmi a1, zero, t1, 0\n\t"
                 "aes32dsmi a1, a1, t0, 1\n\t"
                 "aes32dsmi a1, a1, t3, 2\n\t"
                 "aes32dsmi a1, a1, t2, 3\n\t"
                 "aes32dsmi a2, zero, t2, 0\n\t"
                 "aes32dsmi a2, a2, t1, 1\n\t"
                 "aes32dsmi a2, a2, t0, 2\n\t"
                 "aes32dsmi a2, a2, t3, 3\n\t"
                 "aes32dsmi a3, zero, t3, 0\n\t"
                 "aes32dsmi a3, a3, t2, 1\n\t"
                 "aes32dsmi a3, a3, t1, 2\n\t"
                 "aes32dsmi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[0]), "r"(&key_col[1]), "r"(&key_col[2]), "r"(&key_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 10
    asm volatile("aes32dsi a0, zero, t0, 0\n\t"
                 "aes32dsi a0, a0, t3, 1\n\t"
                 "aes32dsi a0, a0, t2, 2\n\t"
                 "aes32dsi a0, a0, t1, 3\n\t"
                 "aes32dsi a1, zero, t1, 0\n\t"
                 "aes32dsi a1, a1, t0, 1\n\t"
                 "aes32dsi a1, a1, t3, 2\n\t"
                 "aes32dsi a1, a1, t2, 3\n\t"
                 "aes32dsi a2, zero, t2, 0\n\t"
                 "aes32dsi a2, a2, t1, 1\n\t"
                 "aes32dsi a2, a2, t0, 2\n\t"
                 "aes32dsi a2, a2, t3, 3\n\t"
                 "aes32dsi a3, zero, t3, 0\n\t"
                 "aes32dsi a3, a3, t2, 1\n\t"
                 "aes32dsi a3, a3, t1, 2\n\t"
                 "aes32dsi a3, a3, t0, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "sw t1, 0(%5)\n\t"
                 "sw t2, 0(%6)\n\t"
                 "sw t3, 0(%7)"
                :
                : "r"((uint32_t*)&key[0]), "r"((uint32_t*)&key[4]), "r"((uint32_t*)&key[8]), "r"((uint32_t*)&key[12]), "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3", "memory");

}

// input ciphertext
//  3AD77BB4 0D7A3660 A89ECAF3 2466EF97
block ciphertext = {
    0x3A, 0xD7, 0x7B, 0xB4,
    0x0D, 0x7A, 0x36, 0x60,
    0xA8, 0x9E, 0xCA, 0xF3,
    0x24, 0x66, 0xEF, 0x97
};

// input key
block key = {
    0x2B, 0x7E, 0x15, 0x16,
    0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88,
    0x09, 0xCF, 0x4F, 0x3C
};

int main(){
    //KeySchedule(input_key);
    aes_dec(ciphertext, key);
}