/*
AES-128 Decryption Program with Extensions by Gabriel Camero
To be compiled with extensions
*/

#include<stdint.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

void AES_Decrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t *data_col = (uint32_t*)data;
    uint32_t *key = (uint32_t*)key_schedule[10];
    data_col[0] ^= key[0];
    data_col[1] ^= key[1];
    data_col[2] ^= key[2];
    data_col[3] ^= key[3];

    uint32_t *key_col = (uint32_t*)key_schedule[0];

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
                : "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3]), "r"(&key_col[36]), "r"(&key_col[37]), "r"(&key_col[38]), "r"(&key_col[39])
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
                : "r"(&key_col[32]), "r"(&key_col[33]), "r"(&key_col[34]), "r"(&key_col[35])
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
                : "r"(&key_col[28]), "r"(&key_col[29]), "r"(&key_col[30]), "r"(&key_col[31])
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
                : "r"(&key_col[24]), "r"(&key_col[25]), "r"(&key_col[26]), "r"(&key_col[27])
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
                : "r"(&key_col[20]), "r"(&key_col[21]), "r"(&key_col[22]), "r"(&key_col[23])
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
                : "r"(&key_col[16]), "r"(&key_col[17]), "r"(&key_col[18]), "r"(&key_col[19])
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
                : "r"(&key_col[12]), "r"(&key_col[13]), "r"(&key_col[14]), "r"(&key_col[15])
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
                : "r"(&key_col[8]), "r"(&key_col[9]), "r"(&key_col[10]), "r"(&key_col[11])
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
                : "r"(&key_col[4]), "r"(&key_col[5]), "r"(&key_col[6]), "r"(&key_col[7])
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
                : "r"(&key_col[0]), "r"(&key_col[1]), "r"(&key_col[2]), "r"(&key_col[3]), "r"(&data_col[0]), "r"(&data_col[1]), "r"(&data_col[2]), "r"(&data_col[3])
                : "t0", "t1", "t2", "t3", "a0", "a1", "a2", "a3", "memory");

}

// input ciphertext
//  3AD77BB4 0D7A3660 A89ECAF3 2466EF97
block ciphertext = {
    0x7B, 0x0C, 0x78, 0x5E, 
    0x27, 0xE8, 0xAD, 0x3F, 
    0x82, 0x23, 0x20, 0x71, 
    0x04, 0x72, 0x5D, 0xD4
};

// input key
block in_key_schedule[11] = {
    {
        0x2B, 0x7E, 0x15, 0x16, 
        0x28, 0xAE, 0xD2, 0xA6, 
        0xAB, 0xF7, 0x15, 0x88, 
        0x09, 0xCF, 0x4F, 0x3C
    },
    {
        0x2B, 0x37, 0x08, 0xA7, 
        0xF2, 0x62, 0xD4, 0x05, 
        0xBC, 0x3E, 0xBD, 0xBF, 
        0x4B, 0x61, 0x7D, 0x62
    },
    {
        0xCC, 0x75, 0x05, 0xEB, 
        0x3E, 0x17, 0xD1, 0xEE, 
        0x82, 0x29, 0x6C, 0x51, 
        0xC9, 0x48, 0x11, 0x33
    },
    {
        0x7C, 0x1F, 0x13, 0xF7, 
        0x42, 0x08, 0xC2, 0x19, 
        0xC0, 0x21, 0xAE, 0x48, 
        0x09, 0x69, 0xBF, 0x7B
    },
    {
        0x90, 0x88, 0x44, 0x13, 
        0xD2, 0x80, 0x86, 0x0A, 
        0x12, 0xA1, 0x28, 0x42, 
        0x1B, 0xC8, 0x97, 0x39
    },
    {
        0x6E, 0xA3, 0x0A, 0xFC, 
        0xBC, 0x23, 0x8C, 0xF6, 
        0xAE, 0x82, 0xA4, 0xB4, 
        0xB5, 0x4A, 0x33, 0x8D
    },
    {
        0x6E, 0xFC, 0xD8, 0x76, 
        0xD2, 0xDF, 0x54, 0x80, 
        0x7C, 0x5D, 0xF0, 0x34, 
        0xC9, 0x17, 0xC3, 0xB9
    },
    {
        0x12, 0xC0, 0x76, 0x47, 
        0xC0, 0x1F, 0x22, 0xC7, 
        0xBC, 0x42, 0xD2, 0xF3, 
        0x75, 0x55, 0x11, 0x4A
    },
    {
        0xDF, 0x7D, 0x92, 0x5A, 
        0x1F, 0x62, 0xB0, 0x9D, 
        0xA3, 0x20, 0x62, 0x6E, 
        0xD6, 0x75, 0x73, 0x24
    },
    {
        0x0C, 0x7B, 0x5A, 0x63, 
        0x13, 0x19, 0xEA, 0xFE, 
        0xB0, 0x39, 0x88, 0x90, 
        0x66, 0x4C, 0xFB, 0xB4
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
    AES_Decrypt(ciphertext, in_key_schedule);
}