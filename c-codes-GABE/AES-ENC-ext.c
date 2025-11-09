/*
AES-128 Encryption Program with Extensions by Gabriel Camero
To be compiled with extensions
*/

#include<stdint.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

void AES_Encrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t *data_col = (uint32_t*)data;
    uint32_t *key = (uint32_t*)key_schedule[0];
    data_col[0] ^= key[0];
    data_col[1] ^= key[1];
    data_col[2] ^= key[2];
    data_col[3] ^= key[3];
    
    uint32_t *key_col = (uint32_t*)key_schedule[1];

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

block plaintext = {
    0xF6, 0x9F, 0x24, 0x45,
    0xDF, 0x4F, 0x9B, 0x17,
    0xAD, 0x2B, 0x41, 0x7B,
    0xE6, 0x6C, 0x37, 0x10
};

block in_key_schedule[11] = {
    {
        0x2B, 0x7E, 0x15, 0x16, 
        0x28, 0xAE, 0xD2, 0xA6, 
        0xAB, 0xF7, 0x15, 0x88, 
        0x09, 0xCF, 0x4F, 0x3C
    },
    {
        0xA0, 0xFA, 0xFE, 0x17, 
        0x88, 0x54, 0x2C, 0xB1, 
        0x23, 0xA3, 0x39, 0x39, 
        0x2A, 0x6C, 0x76, 0x05
    },
    {
        0xF2, 0xC2, 0x95, 0xF2, 
        0x7A, 0x96, 0xB9, 0x43, 
        0x59, 0x35, 0x80, 0x7A, 
        0x73, 0x59, 0xF6, 0x7F
    },
    {
        0x3D, 0x80, 0x47, 0x7D, 
        0x47, 0x16, 0xFE, 0x3E, 
        0x1E, 0x23, 0x7E, 0x44, 
        0x6D, 0x7A, 0x88, 0x3B
    },
    {
        0xEF, 0x44, 0xA5, 0x41, 
        0xA8, 0x52, 0x5B, 0x7F, 
        0xB6, 0x71, 0x25, 0x3B, 
        0xDB, 0x0B, 0xAD, 0x00
    },
    {
        0xD4, 0xD1, 0xC6, 0xF8, 
        0x7C, 0x83, 0x9D, 0x87, 
        0xCA, 0xF2, 0xB8, 0xBC, 
        0x11, 0xF9, 0x15, 0xBC
    },
    {
        0x6D, 0x88, 0xA3, 0x7A, 
        0x11, 0x0B, 0x3E, 0xFD, 
        0xDB, 0xF9, 0x86, 0x41, 
        0xCA, 0x00, 0x93, 0xFD
    },
    {
        0x4E, 0x54, 0xF7, 0x0E, 
        0x5F, 0x5F, 0xC9, 0xF3, 
        0x84, 0xA6, 0x4F, 0xB2, 
        0x4E, 0xA6, 0xDC, 0x4F
    },
    {
        0xEA, 0xD2, 0x73, 0x21, 
        0xB5, 0x8D, 0xBA, 0xD2, 
        0x31, 0x2B, 0xF5, 0x60, 
        0x7F, 0x8D, 0x29, 0x2F
    },
    {
        0xAC, 0x77, 0x66, 0xF3, 
        0x19, 0xFA, 0xDC, 0x21, 
        0x28, 0xD1, 0x29, 0x41, 
        0x57, 0x5C, 0x00, 0x6E
    },
    {
        0xD0, 0x14, 0xF9, 0xA8, 
        0xC9, 0xEE, 0x25, 0x89, 
        0xE1, 0x3F, 0x0C, 0xC8, 
        0xB6, 0x63, 0x0C, 0xA6
    }
};

int main(){
    //KeySchedule(input_key);
    AES_Encrypt(plaintext, in_key_schedule);
}