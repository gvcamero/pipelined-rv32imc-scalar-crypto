#include<stdint.h>
#include<stdio.h>
#include<string.h>

// Global SBoxes
static const uint8_t rsbox[256] = {

    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB, // V
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB, // V
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E, // V
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25, // V
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92, // V
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84, // V
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06, // V
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B, // V
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73, // V
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E, // V
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B, // V
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4, // V
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F, // V
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF, // V
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61, // V
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D // V

};
const uint8_t fsbox[256] = {

    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16

};

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// KeySchedule function
// Inputs: pointer to  array of 11 keys, (first block: input key; the rest are empty)
// Output: entire key schedule in the rest of the array
void KeySchedule(block key_schedule[11]){
    
    uint32_t* key_sched_col = (uint32_t*)key_schedule[0];

    // Key 1
    key_schedule[1][0] = fsbox[key_schedule[0][13]];
    key_schedule[1][1] = fsbox[key_schedule[0][14]];
    key_schedule[1][2] = fsbox[key_schedule[0][15]];
    key_schedule[1][3] = fsbox[key_schedule[0][12]];

    /* Sequence:
    %0 = address of new key, first column
    %1 = value of previous key, first column
    %2 = value of previous key, second column
    %3 = address of new key, second column
    %4 = value of previous key, third column
    %5 = address of new key, third column
    %6 = value of previous key, fourth column
    %7 = address of new key, fourth column
    */ 
    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x01\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[4]), "r"(key_sched_col[0]), "r"(key_sched_col[1]), "r"(&key_sched_col[5]), "r"(key_sched_col[2]), "r"(&key_sched_col[6]), "r"(key_sched_col[3]), "r"(&key_sched_col[7])
                : "t0", "memory");
    
    // Key 2
    key_schedule[2][0] = fsbox[key_schedule[1][13]];
    key_schedule[2][1] = fsbox[key_schedule[1][14]];
    key_schedule[2][2] = fsbox[key_schedule[1][15]];
    key_schedule[2][3] = fsbox[key_schedule[1][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x02\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[8]), "r"(key_sched_col[4]), "r"(key_sched_col[5]), "r"(&key_sched_col[9]), "r"(key_sched_col[6]), "r"(&key_sched_col[10]), "r"(key_sched_col[7]), "r"(&key_sched_col[11])
                : "t0", "memory");

    // Key 3
    key_schedule[3][0] = fsbox[key_schedule[2][13]];
    key_schedule[3][1] = fsbox[key_schedule[2][14]];
    key_schedule[3][2] = fsbox[key_schedule[2][15]];
    key_schedule[3][3] = fsbox[key_schedule[2][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x04\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[12]), "r"(key_sched_col[8]), "r"(key_sched_col[9]), "r"(&key_sched_col[13]), "r"(key_sched_col[10]), "r"(&key_sched_col[14]), "r"(key_sched_col[11]), "r"(&key_sched_col[15])
                : "t0", "memory");

    // Key 4
    key_schedule[4][0] = fsbox[key_schedule[3][13]];
    key_schedule[4][1] = fsbox[key_schedule[3][14]];
    key_schedule[4][2] = fsbox[key_schedule[3][15]];
    key_schedule[4][3] = fsbox[key_schedule[3][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x08\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[16]), "r"(key_sched_col[12]), "r"(key_sched_col[13]), "r"(&key_sched_col[17]), "r"(key_sched_col[14]), "r"(&key_sched_col[18]), "r"(key_sched_col[15]), "r"(&key_sched_col[19])
                : "t0", "memory");

    // Key 5
    key_schedule[5][0] = fsbox[key_schedule[4][13]];
    key_schedule[5][1] = fsbox[key_schedule[4][14]];
    key_schedule[5][2] = fsbox[key_schedule[4][15]];
    key_schedule[5][3] = fsbox[key_schedule[4][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x10\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[20]), "r"(key_sched_col[16]), "r"(key_sched_col[17]), "r"(&key_sched_col[21]), "r"(key_sched_col[18]), "r"(&key_sched_col[22]), "r"(key_sched_col[19]), "r"(&key_sched_col[23])
                : "t0", "memory");

    // Key 6
    key_schedule[6][0] = fsbox[key_schedule[5][13]];
    key_schedule[6][1] = fsbox[key_schedule[5][14]];
    key_schedule[6][2] = fsbox[key_schedule[5][15]];
    key_schedule[6][3] = fsbox[key_schedule[5][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x20\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[24]), "r"(key_sched_col[20]), "r"(key_sched_col[21]), "r"(&key_sched_col[25]), "r"(key_sched_col[22]), "r"(&key_sched_col[26]), "r"(key_sched_col[23]), "r"(&key_sched_col[27])
                : "t0", "memory");

    // Key 7
    key_schedule[7][0] = fsbox[key_schedule[6][13]];
    key_schedule[7][1] = fsbox[key_schedule[6][14]];
    key_schedule[7][2] = fsbox[key_schedule[6][15]];
    key_schedule[7][3] = fsbox[key_schedule[6][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x40\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[28]), "r"(key_sched_col[24]), "r"(key_sched_col[25]), "r"(&key_sched_col[29]), "r"(key_sched_col[26]), "r"(&key_sched_col[30]), "r"(key_sched_col[27]), "r"(&key_sched_col[31])
                : "t0", "memory");

    // Key 8
    key_schedule[8][0] = fsbox[key_schedule[7][13]];
    key_schedule[8][1] = fsbox[key_schedule[7][14]];
    key_schedule[8][2] = fsbox[key_schedule[7][15]];
    key_schedule[8][3] = fsbox[key_schedule[7][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x80\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[32]), "r"(key_sched_col[28]), "r"(key_sched_col[29]), "r"(&key_sched_col[33]), "r"(key_sched_col[30]), "r"(&key_sched_col[34]), "r"(key_sched_col[31]), "r"(&key_sched_col[35])
                : "t0", "memory");

    // Key 9
    key_schedule[9][0] = fsbox[key_schedule[8][13]];
    key_schedule[9][1] = fsbox[key_schedule[8][14]];
    key_schedule[9][2] = fsbox[key_schedule[8][15]];
    key_schedule[9][3] = fsbox[key_schedule[8][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x1B\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[36]), "r"(key_sched_col[32]), "r"(key_sched_col[33]), "r"(&key_sched_col[37]), "r"(key_sched_col[34]), "r"(&key_sched_col[38]), "r"(key_sched_col[35]), "r"(&key_sched_col[39])
                : "t0", "memory");

    // Key 10
    key_schedule[10][0] = fsbox[key_schedule[9][13]];
    key_schedule[10][1] = fsbox[key_schedule[9][14]];
    key_schedule[10][2] = fsbox[key_schedule[9][15]];
    key_schedule[10][3] = fsbox[key_schedule[9][12]];

    asm volatile("lw t0, 0(%0)\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x36\n\t"
                 "sw t0, 0(%0)\n\t"
                 "xor t0, t0, %2\n\t"
                 "sw t0, 0(%3)\n\t"
                 "xor t0, t0, %4\n\t"
                 "sw t0, 0(%5)\n\t"
                 "xor t0, t0, %6\n\t"
                 "sw t0, 0(%7)"
                :
                : "r"(&key_sched_col[40]), "r"(key_sched_col[36]), "r"(key_sched_col[37]), "r"(&key_sched_col[41]), "r"(key_sched_col[38]), "r"(&key_sched_col[42]), "r"(key_sched_col[39]), "r"(&key_sched_col[43])
                : "t0", "memory");

}

// Function for Galois Field Multiplication by 2, used in MixColumns
// Input: byte value
// Output: GF(2^8) multiplication by 2 result of the input
uint8_t xTIMES(uint8_t byte){
    return (byte << 1) ^ ((byte & 0x80) ? 0x1B : 0x00);
}

// Functions for GF(2^8) multiplication by E, B, D, 9 for InvMixColumns
uint8_t xTIMES_E(uint8_t byte){
    uint8_t x2 = xTIMES(byte);
    uint8_t x4 = xTIMES(x2);
    uint8_t x8 = xTIMES(x4);
    return x8 ^ x4 ^ x2;
}
uint8_t xTIMES_B(uint8_t byte){
    uint8_t x2 = xTIMES(byte);
    uint8_t x8 = xTIMES(xTIMES(x2));
    return x8 ^ x2 ^ byte;
}
uint8_t xTIMES_D(uint8_t byte){
    uint8_t x4 = xTIMES(xTIMES(byte));
    uint8_t x8 = xTIMES(x4);
    return x8 ^ x4 ^ byte;
}
uint8_t xTIMES_9(uint8_t byte){
    return xTIMES(xTIMES(xTIMES(byte))) ^ byte;
}

// Function that performs InvSubBytes, InvShiftRows, AddRoundKey, InvMixColumns
// Input: data block, round key
// Output: transformation of the data block
void InvSubShiftARKMix(block data, block round_key){

    block tmp;
    // InvSubBytes, InvShiftRows
    tmp[0]  = rsbox[data[0]];
    tmp[1]  = rsbox[data[13]];
    tmp[2]  = rsbox[data[10]];
    tmp[3]  = rsbox[data[7]];

    tmp[4]  = rsbox[data[4]];
    tmp[5]  = rsbox[data[1]];
    tmp[6]  = rsbox[data[14]];
    tmp[7]  = rsbox[data[11]];

    tmp[8]  = rsbox[data[8]];
    tmp[9]  = rsbox[data[5]];
    tmp[10] = rsbox[data[2]];
    tmp[11] = rsbox[data[15]];

    tmp[12] = rsbox[data[12]];
    tmp[13] = rsbox[data[9]];
    tmp[14] = rsbox[data[6]];
    tmp[15] = rsbox[data[3]];

    // AddRoundKey, InvMixColumns

    uint32_t* tmp_col = (uint32_t*)tmp;
    uint32_t* key_col = (uint32_t*)round_key;
    tmp_col[0] = tmp_col[0] ^ key_col[0];
    tmp_col[1] = tmp_col[1] ^ key_col[1];
    tmp_col[2] = tmp_col[2] ^ key_col[2];
    tmp_col[3] = tmp_col[3] ^ key_col[3];

    // Column 1
    data[0] = xTIMES_E(tmp[0]) ^ xTIMES_B(tmp[1]) ^ xTIMES_D(tmp[2]) ^ xTIMES_9(tmp[3]);
    data[1] = xTIMES_E(tmp[1]) ^ xTIMES_B(tmp[2]) ^ xTIMES_D(tmp[3]) ^ xTIMES_9(tmp[0]);
    data[2] = xTIMES_E(tmp[2]) ^ xTIMES_B(tmp[3]) ^ xTIMES_D(tmp[0]) ^ xTIMES_9(tmp[1]);
    data[3] = xTIMES_E(tmp[3]) ^ xTIMES_B(tmp[0]) ^ xTIMES_D(tmp[1]) ^ xTIMES_9(tmp[2]);

    // Column 2
    data[4] = xTIMES_E(tmp[4]) ^ xTIMES_B(tmp[5]) ^ xTIMES_D(tmp[6]) ^ xTIMES_9(tmp[7]);
    data[5] = xTIMES_E(tmp[5]) ^ xTIMES_B(tmp[6]) ^ xTIMES_D(tmp[7]) ^ xTIMES_9(tmp[4]);
    data[6] = xTIMES_E(tmp[6]) ^ xTIMES_B(tmp[7]) ^ xTIMES_D(tmp[4]) ^ xTIMES_9(tmp[5]);
    data[7] = xTIMES_E(tmp[7]) ^ xTIMES_B(tmp[4]) ^ xTIMES_D(tmp[5]) ^ xTIMES_9(tmp[6]);

    // Column 3
    data[8] = xTIMES_E(tmp[8]) ^ xTIMES_B(tmp[9]) ^ xTIMES_D(tmp[10]) ^ xTIMES_9(tmp[11]);
    data[9] = xTIMES_E(tmp[9]) ^ xTIMES_B(tmp[10]) ^ xTIMES_D(tmp[11]) ^ xTIMES_9(tmp[8]);
    data[10] = xTIMES_E(tmp[10]) ^ xTIMES_B(tmp[11]) ^ xTIMES_D(tmp[8]) ^ xTIMES_9(tmp[9]);
    data[11] = xTIMES_E(tmp[11]) ^ xTIMES_B(tmp[8]) ^ xTIMES_D(tmp[9]) ^ xTIMES_9(tmp[10]);

    // Column 4
    data[12] = xTIMES_E(tmp[12]) ^ xTIMES_B(tmp[13]) ^ xTIMES_D(tmp[14]) ^ xTIMES_9(tmp[15]);
    data[13] = xTIMES_E(tmp[13]) ^ xTIMES_B(tmp[14]) ^ xTIMES_D(tmp[15]) ^ xTIMES_9(tmp[12]);
    data[14] = xTIMES_E(tmp[14]) ^ xTIMES_B(tmp[15]) ^ xTIMES_D(tmp[12]) ^ xTIMES_9(tmp[13]);
    data[15] = xTIMES_E(tmp[15]) ^ xTIMES_B(tmp[12]) ^ xTIMES_D(tmp[13]) ^ xTIMES_9(tmp[14]);

}

// InvSubBytes, InvShiftRows, AddRoundKey (for last round)
void InvSubShiftARK(block data, block round_key){

    block tmp;
    // InvSubBytes, InvShiftRows
    tmp[0]  = rsbox[data[0]];
    tmp[1]  = rsbox[data[13]];
    tmp[2]  = rsbox[data[10]];
    tmp[3]  = rsbox[data[7]];

    tmp[4]  = rsbox[data[4]];
    tmp[5]  = rsbox[data[1]];
    tmp[6]  = rsbox[data[14]];
    tmp[7]  = rsbox[data[11]];

    tmp[8]  = rsbox[data[8]];
    tmp[9]  = rsbox[data[5]];
    tmp[10] = rsbox[data[2]];
    tmp[11] = rsbox[data[15]];

    tmp[12] = rsbox[data[12]];
    tmp[13] = rsbox[data[9]];
    tmp[14] = rsbox[data[6]];
    tmp[15] = rsbox[data[3]];

    // AddRoundKey

    uint32_t* data_col = (uint32_t*)data;
    uint32_t* tmp_col = (uint32_t*)tmp;
    uint32_t* key_col = (uint32_t*)round_key;
    data_col[0] = tmp_col[0] ^ key_col[0];
    data_col[1] = tmp_col[1] ^ key_col[1];
    data_col[2] = tmp_col[2] ^ key_col[2];
    data_col[3] = tmp_col[3] ^ key_col[3];

}

// main AES Decryption function
// Inputs: block to be decrypted, array of input key + key schedule
// Output: decrypted block
void AES_Decrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t* dst = (uint32_t*)data;
    uint32_t* key = (uint32_t*)key_schedule[10];
    dst[0] ^= key[0];
    dst[1] ^= key[1];
    dst[2] ^= key[2];
    dst[3] ^= key[3];

    // Rounds 1-9
    InvSubShiftARKMix(data, key_schedule[9]);
    InvSubShiftARKMix(data, key_schedule[8]);
    InvSubShiftARKMix(data, key_schedule[7]);
    InvSubShiftARKMix(data, key_schedule[6]);
    InvSubShiftARKMix(data, key_schedule[5]);
    InvSubShiftARKMix(data, key_schedule[4]);
    InvSubShiftARKMix(data, key_schedule[3]);
    InvSubShiftARKMix(data, key_schedule[2]);
    InvSubShiftARKMix(data, key_schedule[1]);

    // Final Round
    InvSubShiftARK(data, key_schedule[0]);

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
block in_ciphertext[4] = {
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
    }
};

// Global output
block out_plaintext[4];

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
    CBC(in_ciphertext, out_plaintext, 4, in_key, init_vector);
}