/*
AES-128 Encryption Program with Extensions by Gabriel Camero
To be verified
*/

#include<stdint.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// Global variables
block key_schedule[10]; // Round keys array

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
    

    // LOOP VERSION
    /*
    uint32_t *old_key_col;
    uint32_t *new_key_col;

    const uint32_t rcon[10] = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
    };

    for(int i = 0; i < 10; i++){
        if(i == 0){
            old_key_col = (uint32_t*)&key[0];
            new_key_col = (uint32_t*)&key_schedule[0][0];
        }
        else{
            old_key_col = (uint32_t*)&key_schedule[i-1][0];
            new_key_col = (uint32_t*)&key_schedule[i][0];
        }
        asm volatile("rori t1, %0, 8\n\t"
                     "aes32esi t0, zero, t1, 0\n\t"
                     "aes32esi t0, t0, t1, 1\n\t"
                     "aes32esi t0, t0, t1, 2\n\t"
                     "aes32esi t0, t0, t1, 3\n\t"
                     "xor t0, t0, %1\n\t"
                     "xor t0, t0, %8\n\t"
                     "sw t0, 0(%2)\n\t"
                     "xor t0, t0, %3\n\t"
                     "sw t0, 0(%4)\n\t"
                     "xor t0, t0, %5\n\t"
                     "sw t0, 0(%6)\n\t"
                     "xor t0, t0, %0\n\t"
                     "sw t0, 0(%7)"
                    : 
                    : "r"(old_key_col[3]), "r"(old_key_col[0]), "r"(&new_key_col[0]), "r"(old_key_col[1]), "r"(&new_key_col[1]), "r"(old_key_col[2]), "r"(&new_key_col[2]), "r"(&new_key_col[3]), "r"(rcon[i])
                    : "t1", "t0", "memory");
    }
    */
    

}

void aes_enc(block data, block key){
    
    // KeySchedule
    KeySchedule(key);

    // Initial AddRoundKey
    for(int i = 0; i < 16; i++){
        data[i] ^= key[i];
    }
    
    uint32_t *data_col = (uint32_t*)&data[0];
    uint32_t *key_col = (uint32_t*)&key_schedule[0][0];

    // Round 1
    asm volatile("lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "lw t0, 0(%4)\n\t"
                 "lw t1, 0(%5)\n\t"
                 "lw t2, 0(%6)\n\t"
                 "lw t3, 0(%7)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3]), "r"(&key_col[0]), "r"(&key_col[1]), "r"(&key_col[2]), "r"(&key_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");
    
    // Round 2
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 3
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 4
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 5
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 7
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 8
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
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

    // Round 9
    asm volatile("aes32esmi a0, zero, t0, 0\n\t"
                 "aes32esmi a0, a0, t1, 1\n\t"
                 "aes32esmi a0, a0, t2, 2\n\t"
                 "aes32esmi a0, a0, t3, 3\n\t"
                 "aes32esmi a1, zero, t1, 0\n\t"
                 "aes32esmi a1, a1, t2, 1\n\t"
                 "aes32esmi a1, a1, t3, 2\n\t"
                 "aes32esmi a1, a1, t0, 3\n\t"
                 "aes32esmi a2, zero, t2, 0\n\t"
                 "aes32esmi a2, a2, t3, 1\n\t"
                 "aes32esmi a2, a2, t0, 2\n\t"
                 "aes32esmi a2, a2, t1, 3\n\t"
                 "aes32esmi a3, zero, t3, 0\n\t"
                 "aes32esmi a3, a3, t0, 1\n\t"
                 "aes32esmi a3, a3, t1, 2\n\t"
                 "aes32esmi a3, a3, t2, 3\n\t"
                 "lw t0, 0(%0)\n\t"
                 "lw t1, 0(%1)\n\t"
                 "lw t2, 0(%2)\n\t"
                 "lw t3, 0(%3)\n\t"
                 "xor t0, a0, t0\n\t"
                 "xor t1, a1, t1\n\t"
                 "xor t2, a2, t2\n\t"
                 "xor t3, a3, t3"
                :
                : "r"(&key_col[32]), "r"(&key_col[33]), "r"(&key_col[34]), "r"(&key_col[35])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3");

    // Round 10
    asm volatile("aes32esi a0, zero, t0, 0\n\t"
                 "aes32esi a0, a0, t1, 1\n\t"
                 "aes32esi a0, a0, t2, 2\n\t"
                 "aes32esi a0, a0, t3, 3\n\t"
                 "aes32esi a1, zero, t1, 0\n\t"
                 "aes32esi a1, a1, t2, 1\n\t"
                 "aes32esi a1, a1, t3, 2\n\t"
                 "aes32esi a1, a1, t0, 3\n\t"
                 "aes32esi a2, zero, t2, 0\n\t"
                 "aes32esi a2, a2, t3, 1\n\t"
                 "aes32esi a2, a2, t0, 2\n\t"
                 "aes32esi a2, a2, t1, 3\n\t"
                 "aes32esi a3, zero, t3, 0\n\t"
                 "aes32esi a3, a3, t0, 1\n\t"
                 "aes32esi a3, a3, t1, 2\n\t"
                 "aes32esi a3, a3, t2, 3\n\t"
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
                : "r"(&key_col[36]), "r"(&key_col[37]), "r"(&key_col[38]), "r"(&key_col[39]), "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3", "memory");

}

// input plaintext
block plaintext = {
    0xF6, 0x9F, 0x24, 0x45,
    0xDF, 0x4F, 0x9B, 0x17,
    0xAD, 0x2B, 0x41, 0x7B,
    0xE6, 0x6C, 0x37, 0x10
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
    aes_enc(plaintext, key);
}