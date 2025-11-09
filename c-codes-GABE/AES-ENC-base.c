/*
AES-128 Encryption Program by Gabriel Camero
To be compiled with base RV32 only
*/

#include<stdio.h>
#include<stdint.h>

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

// Function for Galois Field Multiplication by 2
uint8_t xTIMES(uint8_t byte){
    return (byte << 1) ^ ((byte & 0x80) ? 0x1B : 0x00);
}

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
    data[0] = xTIMES(tmp[0]) ^ (tmp[1] ^ xTIMES(tmp[1])) ^ tmp[2] ^ tmp[3] ^ round_key[0];
    data[1] = xTIMES(tmp[1]) ^ (tmp[2] ^ xTIMES(tmp[2])) ^ tmp[3] ^ tmp[0] ^ round_key[1];
    data[2] = xTIMES(tmp[2]) ^ (tmp[3] ^ xTIMES(tmp[3])) ^ tmp[0] ^ tmp[1] ^ round_key[2];
    data[3] = xTIMES(tmp[3]) ^ (tmp[0] ^ xTIMES(tmp[0])) ^ tmp[1] ^ tmp[2] ^ round_key[3];

    // Column 2
    data[4] = xTIMES(tmp[4]) ^ (tmp[5] ^ xTIMES(tmp[5])) ^ tmp[6] ^ tmp[7] ^ round_key[4];
    data[5] = xTIMES(tmp[5]) ^ (tmp[6] ^ xTIMES(tmp[6])) ^ tmp[7] ^ tmp[4] ^ round_key[5];
    data[6] = xTIMES(tmp[6]) ^ (tmp[7] ^ xTIMES(tmp[7])) ^ tmp[4] ^ tmp[5] ^ round_key[6];
    data[7] = xTIMES(tmp[7]) ^ (tmp[4] ^ xTIMES(tmp[4])) ^ tmp[5] ^ tmp[6] ^ round_key[7];

    // Column 3
    data[8] = xTIMES(tmp[8]) ^ (tmp[9] ^ xTIMES(tmp[9])) ^ tmp[10] ^ tmp[11] ^ round_key[8];
    data[9] = xTIMES(tmp[9]) ^ (tmp[10] ^ xTIMES(tmp[10])) ^ tmp[11] ^ tmp[8] ^ round_key[9];
    data[10] = xTIMES(tmp[10]) ^ (tmp[11] ^ xTIMES(tmp[11])) ^ tmp[8] ^ tmp[9] ^ round_key[10];
    data[11] = xTIMES(tmp[11]) ^ (tmp[8] ^ xTIMES(tmp[8])) ^ tmp[9] ^ tmp[10] ^ round_key[11];

    // Column 4
    data[12] = xTIMES(tmp[12]) ^ (tmp[13] ^ xTIMES(tmp[13])) ^ tmp[14] ^ tmp[15] ^ round_key[12];
    data[13] = xTIMES(tmp[13]) ^ (tmp[14] ^ xTIMES(tmp[14])) ^ tmp[15] ^ tmp[12] ^ round_key[13];
    data[14] = xTIMES(tmp[14]) ^ (tmp[15] ^ xTIMES(tmp[15])) ^ tmp[12] ^ tmp[13] ^ round_key[14];
    data[15] = xTIMES(tmp[15]) ^ (tmp[12] ^ xTIMES(tmp[12])) ^ tmp[13] ^ tmp[14] ^ round_key[15];

    /*
    // SubBytes
    for(int i = 0; i < 16; i++){
        data[i] = fsbox[data[i]];
    }

    block temp_block; // Provide a memory space for block copies to be used in MixColumns
    uint32_t* temp_block_ptr = (uint32_t*)temp_block; // pointer for temp_block words

    // ShiftRows, MixColumns, AddRoundKey
    // copy original block to save contents
    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];

    for (int c = 0; c < 16; c += 4){

        uint8_t b0 = temp_block[c];
        uint8_t b1 = temp_block[(c + 5) & 0xF];
        uint8_t b2 = temp_block[(c + 10) & 0xF];
        uint8_t b3 = temp_block[(c + 15) & 0xF];

        data[c] = xTIMES(b0) ^ (b1 ^ xTIMES(b1)) ^ b2 ^ b3 ^ round_key[c];
        data[c + 1] = b0 ^ xTIMES(b1) ^ (b2 ^ xTIMES(b2)) ^ b3 ^ round_key[c + 1];
        data[c + 2] = b0 ^ b1 ^ xTIMES(b2) ^ (b3 ^ xTIMES(b3)) ^ round_key[c + 2];
        data[c + 3] = (b0 ^ xTIMES(b0)) ^ b1 ^ b2 ^ xTIMES(b3) ^ round_key[c + 3];
    }
    */

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

    /*
    // SubBytes
    for(int i = 0; i < 16; i++){
        data[i] = fsbox[data[i]];
    }

    block temp_block; // Provide a memory space for block copies
    uint32_t* temp_block_ptr = (uint32_t*)temp_block; // pointer for temp_block words
    
    // ShiftRows, MixColumns, AddRoundKey
    // copy original block to save contents
    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];

    data[0] ^= round_key[0];
    data[1] = temp_block[5] ^ round_key[1];
    data[2] = temp_block[10] ^ round_key[2];
    data[3] = temp_block[15] ^ round_key[3];

    data[4] ^= round_key[4];
    data[5] = temp_block[9] ^ round_key[5];
    data[6] = temp_block[14] ^ round_key[6];
    data[7] = temp_block[3] ^ round_key[7];

    data[8] ^= round_key[8];
    data[9] = temp_block[13] ^ round_key[9];
    data[10] = temp_block[2] ^ round_key[10];
    data[11] = temp_block[7] ^ round_key[11];

    data[12] ^= round_key[12];
    data[13] = temp_block[1] ^ round_key[13];
    data[14] = temp_block[6] ^ round_key[14];
    data[15] = temp_block[11] ^ round_key[15];
    */

}

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
    /*printf("Plaintext: ");
    for (int i = 0; i < 16; i++){
        if ((i == 3) || (i == 7) || (i == 11)){
            printf("%02X ", plaintext[i]);
        } else{
            printf("%02X", plaintext[i]);
        }
    }
    printf("\n");
    printf("Key: ");
    for (int i = 0; i < 16; i++){
        if ((i == 3) || (i == 7) || (i == 11)){
            printf("%02X ", in_key_schedule[0][i]);
        } else{
            printf("%02X", in_key_schedule[0][i]);
        }
    }
    printf("\n");*/
    AES_Encrypt(plaintext, in_key_schedule);
    /*printf("Ciphertext: ");
    for (int i = 0; i < 16; i++){
        if ((i == 3) || (i == 7) || (i == 11)){
            printf("%02X ", plaintext[i]);
        } else{
            printf("%02X", plaintext[i]);
        }
    }
    printf("\n");*/
}