#include<stdint.h>
#include<stdio.h>
#include<string.h>

// Global SBox
static const uint8_t fsbox[256] = {

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

// Function that performs SubBytes, ShiftRows, MixColumns, AddRoundKey
// Input: data block, round key
// Output: transformation of the data block
void SubShiftMixARK(block data, block round_key){

    block tmp;
    // SubBytes, ShiftRows
    tmp[0]  = fsbox[data[0]];
    tmp[1]  = fsbox[data[5]];
    tmp[2]  = fsbox[data[10]];
    tmp[3]  = fsbox[data[15]];

    tmp[4]  = fsbox[data[4]];
    tmp[5]  = fsbox[data[9]];
    tmp[6]  = fsbox[data[14]];
    tmp[7]  = fsbox[data[3]];

    tmp[8]  = fsbox[data[8]];
    tmp[9]  = fsbox[data[13]];
    tmp[10] = fsbox[data[2]];
    tmp[11] = fsbox[data[7]];

    tmp[12] = fsbox[data[12]];
    tmp[13] = fsbox[data[1]];
    tmp[14] = fsbox[data[6]];
    tmp[15] = fsbox[data[11]];

    // MixColumns, AddRoundKey

    // Column 1
    data[0] = xTIMES(tmp[0]) ^ (tmp[1] ^ xTIMES(tmp[1])) ^ tmp[2] ^ tmp[3];
    data[1] = xTIMES(tmp[1]) ^ (tmp[2] ^ xTIMES(tmp[2])) ^ tmp[3] ^ tmp[0];
    data[2] = xTIMES(tmp[2]) ^ (tmp[3] ^ xTIMES(tmp[3])) ^ tmp[0] ^ tmp[1];
    data[3] = xTIMES(tmp[3]) ^ (tmp[0] ^ xTIMES(tmp[0])) ^ tmp[1] ^ tmp[2];

    // Column 2
    data[4] = xTIMES(tmp[4]) ^ (tmp[5] ^ xTIMES(tmp[5])) ^ tmp[6] ^ tmp[7];
    data[5] = xTIMES(tmp[5]) ^ (tmp[6] ^ xTIMES(tmp[6])) ^ tmp[7] ^ tmp[4];
    data[6] = xTIMES(tmp[6]) ^ (tmp[7] ^ xTIMES(tmp[7])) ^ tmp[4] ^ tmp[5];
    data[7] = xTIMES(tmp[7]) ^ (tmp[4] ^ xTIMES(tmp[4])) ^ tmp[5] ^ tmp[6];

    // Column 3
    data[8] = xTIMES(tmp[8]) ^ (tmp[9] ^ xTIMES(tmp[9])) ^ tmp[10] ^ tmp[11];
    data[9] = xTIMES(tmp[9]) ^ (tmp[10] ^ xTIMES(tmp[10])) ^ tmp[11] ^ tmp[8];
    data[10] = xTIMES(tmp[10]) ^ (tmp[11] ^ xTIMES(tmp[11])) ^ tmp[8] ^ tmp[9];
    data[11] = xTIMES(tmp[11]) ^ (tmp[8] ^ xTIMES(tmp[8])) ^ tmp[9] ^ tmp[10];

    // Column 4
    data[12] = xTIMES(tmp[12]) ^ (tmp[13] ^ xTIMES(tmp[13])) ^ tmp[14] ^ tmp[15];
    data[13] = xTIMES(tmp[13]) ^ (tmp[14] ^ xTIMES(tmp[14])) ^ tmp[15] ^ tmp[12];
    data[14] = xTIMES(tmp[14]) ^ (tmp[15] ^ xTIMES(tmp[15])) ^ tmp[12] ^ tmp[13];
    data[15] = xTIMES(tmp[15]) ^ (tmp[12] ^ xTIMES(tmp[12])) ^ tmp[13] ^ tmp[14];

    uint32_t* data_col = (uint32_t*)data;
    uint32_t* key_col = (uint32_t*)round_key;
    data_col[0] = data_col[0] ^ key_col[0];
    data_col[1] = data_col[1] ^ key_col[1];
    data_col[2] = data_col[2] ^ key_col[2];
    data_col[3] = data_col[3] ^ key_col[3];

}

// SubBytes, ShiftRows and AddRoundKey (for last round)
void SubShiftARK(block data, block round_key){

    block tmp;
    // SubBytes, ShiftRows
    tmp[0]  = fsbox[data[0]];
    tmp[1]  = fsbox[data[5]];
    tmp[2]  = fsbox[data[10]];
    tmp[3]  = fsbox[data[15]];

    tmp[4]  = fsbox[data[4]];
    tmp[5]  = fsbox[data[9]];
    tmp[6]  = fsbox[data[14]];
    tmp[7]  = fsbox[data[3]];

    tmp[8]  = fsbox[data[8]];
    tmp[9]  = fsbox[data[13]];
    tmp[10] = fsbox[data[2]];
    tmp[11] = fsbox[data[7]];

    tmp[12] = fsbox[data[12]];
    tmp[13] = fsbox[data[1]];
    tmp[14] = fsbox[data[6]];
    tmp[15] = fsbox[data[11]];

    // AddRoundKey
    uint32_t* dst = (uint32_t*)data;
    uint32_t* src = (uint32_t*)tmp;
    uint32_t* key = (uint32_t*)round_key;

    dst[0] = src[0] ^ key[0];
    dst[1] = src[1] ^ key[1];
    dst[2] = src[2] ^ key[2];
    dst[3] = src[3] ^ key[3];


}

// main AES Encryption function
// Inputs: block to be encrypted, array of input key + key schedule
// Output: encrypted block
void AES_Encrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t* dst = (uint32_t*)data;
    uint32_t* key = (uint32_t*)key_schedule[0];
    dst[0] ^= key[0];
    dst[1] ^= key[1];
    dst[2] ^= key[2];
    dst[3] ^= key[3];

    // Rounds 1-9
    SubShiftMixARK(data, key_schedule[1]);
    SubShiftMixARK(data, key_schedule[2]);
    SubShiftMixARK(data, key_schedule[3]);
    SubShiftMixARK(data, key_schedule[4]);
    SubShiftMixARK(data, key_schedule[5]);
    SubShiftMixARK(data, key_schedule[6]);
    SubShiftMixARK(data, key_schedule[7]);
    SubShiftMixARK(data, key_schedule[8]);
    SubShiftMixARK(data, key_schedule[9]);

    // Final Round
    SubShiftARK(data, key_schedule[10]);

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
block in_plaintext[50] = {
    {
        0x6b, 0xc1, 0xbe, 0xe2, 
        0x2e, 0x40, 0x9f, 0x96, 
        0xe9, 0x3d, 0x7e, 0x11, 
        0x73, 0x93, 0x17, 0x2a
    },
    {
        0xae, 0x2d, 0x8a, 0x57, 
        0x1e, 0x03, 0xac, 0x9c, 
        0x9e, 0xb7, 0x6f, 0xac, 
        0x45, 0xaf, 0x8e, 0x51
    },
    {
        0x30, 0xc8, 0x1c, 0x46, 
        0xa3, 0x5c, 0xe4, 0x11, 
        0xe5, 0xfb, 0xc1, 0x19, 
        0x1a, 0x0a, 0x52, 0xef
    },
    {
        0xf6, 0x9f, 0x24, 0x45, 
        0xdf, 0x4f, 0x9b, 0x17, 
        0xad, 0x2b, 0x41, 0x7b, 
        0xe6, 0x6c, 0x37, 0x10
    },
    {
        0xe9, 0x34, 0xdd, 0x51, 
        0x53, 0xb2, 0x7e, 0x9f, 
        0xf9, 0x5c, 0xa7, 0x53, 
        0x91, 0xd0, 0x26, 0xa7
    },
    {
        0x1f, 0xa8, 0x1e, 0xfd, 
        0xd0, 0x93, 0xdc, 0xac, 
        0xc7, 0xbf, 0xe8, 0xb8, 
        0x5f, 0xc3, 0x9e, 0x3d
    },
    {
        0xf1, 0x66, 0xe6, 0x4e, 
        0x8a, 0x67, 0x55, 0x0c, 
        0x7b, 0x3e, 0xe4, 0x6e, 
        0x49, 0x0b, 0x03, 0xb1
    },
    {
        0x3c, 0x49, 0xaf, 0x9f, 
        0x69, 0x73, 0xa1, 0x59, 
        0x6f, 0xef, 0x0f, 0x65, 
        0xd2, 0x58, 0x06, 0x43
    },
    {
        0x27, 0x6e, 0x23, 0xf0, 
        0x01, 0xdb, 0xe4, 0x57, 
        0x4e, 0x56, 0x02, 0xb4, 
        0x1e, 0x0c, 0xfc, 0x36
    },
    {
        0x35, 0x90, 0x2c, 0x4a, 
        0x78, 0xd1, 0x57, 0x92, 
        0x83, 0x77, 0x6b, 0xda, 
        0x1e, 0xce, 0xdd, 0x27
    },
    // BATCH 2
    {
        0xf3, 0xd5, 0x10, 0x77, 
        0xa4, 0x8b, 0xd4, 0xc2, 
        0x4b, 0x99, 0x15, 0xd8, 
        0x77, 0x02, 0xf3, 0xbb
    },
    {
        0xbe, 0xae, 0xa5, 0x83, 
        0x8a, 0x27, 0x17, 0x88, 
        0x53, 0xcd, 0x40, 0xdc, 
        0x2e, 0x14, 0xfa, 0x13
    },
    {
        0xe4, 0x2d, 0xf4, 0xf5, 
        0xd2, 0xa4, 0xd0, 0xb8, 
        0xf5, 0x00, 0xdf, 0x46, 
        0x4f, 0xaf, 0x1c, 0xdf
    },
    {
        0x16, 0x2b, 0x0b, 0x60, 
        0x98, 0x43, 0x45, 0x0b, 
        0x37, 0x92, 0xcf, 0x88, 
        0x3e, 0xb3, 0xd5, 0xf5
    },
    {
        0xc3, 0x82, 0x98, 0xb6, 
        0x5e, 0x3f, 0x46, 0x5b, 
        0x49, 0x8d, 0xa6, 0x91, 
        0x8e, 0x3b, 0x20, 0xd3
    },
    {
        0xa1, 0x8a, 0xe9, 0x6c, 
        0x8d, 0x93, 0xa1, 0x22, 
        0x78, 0xb3, 0xda, 0x3d, 
        0x66, 0x47, 0x5c, 0xd6
    },
    {
        0x21, 0x5e, 0x8a, 0x80, 
        0x42, 0x74, 0x29, 0x3a, 
        0xcd, 0x2b, 0xa9, 0x3f, 
        0x8d, 0x65, 0xb2, 0x99
    },
    {
        0x7f, 0xff, 0xad, 0x26, 
        0xa5, 0xbf, 0xfa, 0xb0, 
        0xa1, 0x59, 0x19, 0x91, 
        0xd5, 0x87, 0x87, 0x2f
    },
    {
        0xa4, 0x9b, 0x2c, 0xe0, 
        0x60, 0xe3, 0xd0, 0x09, 
        0x22, 0x4e, 0xa1, 0x16, 
        0xc2, 0xe9, 0x9d, 0x5f
    },
    {
        0xe4, 0x2d, 0x4d, 0xad, 
        0x76, 0x96, 0xf3, 0xe8, 
        0x63, 0x5f, 0xa6, 0xf1, 
        0x38, 0xa5, 0xf2, 0xa9
    },
    // BATCH 3
    {
        0xbe, 0x2e, 0x40, 0xc9, 
        0x55, 0x60, 0x93, 0xd6, 
        0xe6, 0x0a, 0xf6, 0x97, 
        0xe2, 0xf4, 0x8a, 0xac
    },
    {
        0xf3, 0x76, 0x88, 0xae, 
        0xfe, 0xab, 0x44, 0x67, 
        0x1a, 0x1c, 0x87, 0x65, 
        0x07, 0x08, 0xe6, 0xb4
    },
    {
        0xee, 0x01, 0x78, 0x35, 
        0xac, 0xa2, 0x56, 0x44, 
        0x59, 0x1d, 0xed, 0x1d, 
        0x7e, 0xb0, 0xa4, 0x29
    },
    {
        0x3f, 0x13, 0x70, 0x52, 
        0x8d, 0x67, 0x6f, 0xb8, 
        0xcd, 0xca, 0x27, 0x47, 
        0xea, 0xa7, 0xe6, 0xe4
    },
    {
        0x2c, 0xf7, 0x63, 0x27, 
        0x19, 0x1a, 0x2a, 0x5b, 
        0x10, 0xca, 0x8e, 0x48, 
        0x99, 0xc8, 0x69, 0x0f
    },
    {
        0x96, 0x93, 0x6e, 0x9d, 
        0xd3, 0x9f, 0xfd, 0xc1, 
        0x63, 0xeb, 0x25, 0x25, 
        0x36, 0x04, 0x34, 0x83
    },
    {
        0xc2, 0x9e, 0x3d, 0xd1, 
        0x3c, 0xc5, 0x92, 0x16, 
        0xa7, 0x41, 0x78, 0x94, 
        0x6e, 0x37, 0x2f, 0x88
    },
    {
        0xed, 0x38, 0x4e, 0xa6, 
        0xa7, 0xe2, 0x17, 0xed, 
        0x92, 0xae, 0x98, 0x0b, 
        0x7b, 0x7e, 0x4a, 0xcb
    },
    {
        0x6d, 0x84, 0xe5, 0x65, 
        0xdd, 0x99, 0xa0, 0x2e, 
        0x4b, 0xf7, 0xd8, 0x4a, 
        0xdd, 0x4e, 0x84, 0x81
    },
    {
        0x89, 0x6c, 0xec, 0x67, 
        0x98, 0x50, 0x2a, 0x38, 
        0xe6, 0x34, 0x0f, 0x47, 
        0x08, 0xf0, 0xd5, 0x43
    },
    // BATCH 4
    {
        0x41, 0xc0, 0x8f, 0x0d, 
        0x6e, 0x29, 0x2e, 0x04, 
        0xd1, 0xa1, 0x0c, 0x12, 
        0xb1, 0xeb, 0x42, 0xf3
    },
    {
        0xb6, 0xfb, 0xe8, 0xe7, 
        0x38, 0x2f, 0xe8, 0x18, 
        0x50, 0x3a, 0x21, 0x8c, 
        0x94, 0x74, 0xc2, 0x88
    },
    {
        0x0e, 0x6e, 0x05, 0x2a, 
        0x90, 0x71, 0x05, 0x5e, 
        0x6b, 0xa9, 0x65, 0xc2, 
        0x97, 0x13, 0xc9, 0x4b
    },
    {
        0x05, 0x24, 0xbc, 0xbf, 
        0x58, 0xe5, 0xad, 0xfb, 
        0x77, 0xf6, 0xc2, 0xc5, 
        0x1d, 0x6b, 0x25, 0x23
    },
    {
        0xea, 0x9c, 0x6e, 0x31, 
        0x18, 0x5d, 0x13, 0x87, 
        0x51, 0x38, 0x88, 0x13, 
        0xde, 0x75, 0x4f, 0x33
    },
    {
        0xc5, 0xf2, 0x68, 0x6c, 
        0xa9, 0x82, 0x65, 0x09, 
        0x11, 0xdf, 0x03, 0xc4, 
        0x28, 0xdf, 0x6a, 0x22
    },
    {
        0xfa, 0x93, 0x89, 0xb5, 
        0xa6, 0xef, 0x27, 0xe5, 
        0x0e, 0xb0, 0x39, 0xa9, 
        0xad, 0xf1, 0x3b, 0x26
    },
    {
        0xe9, 0x89, 0x4a, 0x6f, 
        0x26, 0x9c, 0x73, 0x79, 
        0xf7, 0x0b, 0x21, 0x13, 
        0x96, 0x9d, 0x84, 0x3d
    },
    {
        0x3f, 0x82, 0x8b, 0x0b, 
        0x1c, 0x2a, 0x0a, 0xc4, 
        0x40, 0x3e, 0x90, 0x44, 
        0xb7, 0x57, 0xd3, 0xed
    },
    {
        0x8f, 0x14, 0xf3, 0x3f, 
        0xe1, 0xb9, 0x18, 0xb6, 
        0x56, 0x3c, 0xc2, 0x6b, 
        0x13, 0xa9, 0xa5, 0x37
    },
    // BATCH 5
    {
        0xcf, 0x0d, 0xe9, 0x4e, 
        0xc7, 0x0a, 0xf8, 0x50, 
        0x50, 0x3a, 0xb2, 0x47, 
        0xd8, 0x19, 0x66, 0x94
    },
    {
        0x7a, 0xfe, 0x91, 0xd0, 
        0x84, 0x97, 0x24, 0x4b, 
        0x89, 0x44, 0xbf, 0x91, 
        0x07, 0x37, 0x87, 0x9c
    },
    {
        0xc0, 0xb6, 0xa7, 0x9d, 
        0x1d, 0xf4, 0xe9, 0x6f, 
        0xfb, 0x30, 0x79, 0x4f, 
        0xf6, 0xc8, 0xce, 0xd8
    },
    {
        0x60, 0x77, 0x6d, 0xd8, 
        0xea, 0x91, 0xb9, 0x51, 
        0xf4, 0x20, 0x8b, 0x87, 
        0x2c, 0x86, 0x42, 0x6a
    },
    {
        0x0f, 0x54, 0x73, 0xfe, 
        0xb0, 0xec, 0xbb, 0xf6, 
        0xa5, 0x02, 0xd7, 0x67, 
        0xb4, 0xa4, 0x11, 0x54
    },
    {
        0x70, 0xc6, 0xe1, 0x27, 
        0x33, 0x31, 0xa5, 0x20, 
        0xbe, 0xf1, 0x3d, 0x8c, 
        0x1a, 0x9b, 0xfd, 0x86
    },
    {
        0xee, 0xc2, 0x46, 0xd0, 
        0xad, 0x33, 0x25, 0x40, 
        0x71, 0x46, 0xe0, 0xdd, 
        0x96, 0x09, 0xa3, 0xc3
    },
    {
        0x18, 0x17, 0x32, 0x53, 
        0xc9, 0x1a, 0xaf, 0xdc, 
        0x21, 0x41, 0x41, 0x92, 
        0xc1, 0x57, 0xdb, 0x0e
    },
    {
        0xab, 0x09, 0x0a, 0x2c, 
        0x60, 0x29, 0x76, 0x99, 
        0xd4, 0x1a, 0xf1, 0x54, 
        0xe6, 0x68, 0xcc, 0x02
    },
    {
        0x05, 0x58, 0x0f, 0x7b, 
        0x85, 0xeb, 0x48, 0x1b, 
        0x89, 0xb0, 0x81, 0x03, 
        0xc0, 0x13, 0xad, 0xc0
    }
};

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
    CBC(in_plaintext, 50, in_key, init_vector);
}