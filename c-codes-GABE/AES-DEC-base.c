/*
AES-128 Decryption Program by Gabriel Camero
To be compiled with base RV32 only
*/

//#include<stdio.h>
#include<stdint.h>

// Global SBox
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

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// Function for Galois Field Multiplication by 2
uint8_t xTIMES(uint8_t byte){
    return (byte << 1) ^ ((byte & 0x80) ? 0x1B : 0x00);
}

// Functions for GF(2^8) multiplication by E, B, D, 9 for InvMixColumns
uint8_t xTIMES_E(uint8_t byte){
    // return xTIMES(xTIMES(xTIMES(byte))) ^ xTIMES(xTIMES(byte)) ^ xTIMES(byte);
    uint8_t x2 = xTIMES(byte);
    uint8_t x4 = xTIMES(x2);
    uint8_t x8 = xTIMES(x4);
    return x8 ^ x4 ^ x2;
}
uint8_t xTIMES_B(uint8_t byte){
    // return xTIMES(xTIMES(xTIMES(byte))) ^ xTIMES(byte) ^ byte;
    uint8_t x2 = xTIMES(byte);
    uint8_t x8 = xTIMES(xTIMES(x2));
    return x8 ^ x2 ^ byte;
}
uint8_t xTIMES_D(uint8_t byte){
    // return xTIMES(xTIMES(xTIMES(byte))) ^ xTIMES(xTIMES(byte)) ^ byte;
    uint8_t x4 = xTIMES(xTIMES(byte));
    uint8_t x8 = xTIMES(x4);
    return x8 ^ x4 ^ byte;
}
uint8_t xTIMES_9(uint8_t byte){
    return xTIMES(xTIMES(xTIMES(byte))) ^ byte;
}

void InvSubShiftMixARK(block data, block round_key){

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

    // InvMixColumns, AddRoundKey

    // Column 1
    data[0] = xTIMES_E(tmp[0]) ^ xTIMES_B(tmp[1]) ^ xTIMES_D(tmp[2]) ^ xTIMES_9(tmp[3]) ^ round_key[0];
    data[1] = xTIMES_E(tmp[1]) ^ xTIMES_B(tmp[2]) ^ xTIMES_D(tmp[3]) ^ xTIMES_9(tmp[0]) ^ round_key[1];
    data[2] = xTIMES_E(tmp[2]) ^ xTIMES_B(tmp[3]) ^ xTIMES_D(tmp[0]) ^ xTIMES_9(tmp[1]) ^ round_key[2];
    data[3] = xTIMES_E(tmp[3]) ^ xTIMES_B(tmp[0]) ^ xTIMES_D(tmp[1]) ^ xTIMES_9(tmp[2]) ^ round_key[3];

    // Column 2
    data[4] = xTIMES_E(tmp[4]) ^ xTIMES_B(tmp[5]) ^ xTIMES_D(tmp[6]) ^ xTIMES_9(tmp[7]) ^ round_key[4];
    data[5] = xTIMES_E(tmp[5]) ^ xTIMES_B(tmp[6]) ^ xTIMES_D(tmp[7]) ^ xTIMES_9(tmp[4]) ^ round_key[5];
    data[6] = xTIMES_E(tmp[6]) ^ xTIMES_B(tmp[7]) ^ xTIMES_D(tmp[4]) ^ xTIMES_9(tmp[5]) ^ round_key[6];
    data[7] = xTIMES_E(tmp[7]) ^ xTIMES_B(tmp[4]) ^ xTIMES_D(tmp[5]) ^ xTIMES_9(tmp[6]) ^ round_key[7];

    // Column 3
    data[8] = xTIMES_E(tmp[8]) ^ xTIMES_B(tmp[9]) ^ xTIMES_D(tmp[10]) ^ xTIMES_9(tmp[11]) ^ round_key[8];
    data[9] = xTIMES_E(tmp[9]) ^ xTIMES_B(tmp[10]) ^ xTIMES_D(tmp[11]) ^ xTIMES_9(tmp[8]) ^ round_key[9];
    data[10] = xTIMES_E(tmp[10]) ^ xTIMES_B(tmp[11]) ^ xTIMES_D(tmp[8]) ^ xTIMES_9(tmp[9]) ^ round_key[10];
    data[11] = xTIMES_E(tmp[11]) ^ xTIMES_B(tmp[8]) ^ xTIMES_D(tmp[9]) ^ xTIMES_9(tmp[10]) ^ round_key[11];

    // Column 4
    data[12] = xTIMES_E(tmp[12]) ^ xTIMES_B(tmp[13]) ^ xTIMES_D(tmp[14]) ^ xTIMES_9(tmp[15]) ^ round_key[12];
    data[13] = xTIMES_E(tmp[13]) ^ xTIMES_B(tmp[14]) ^ xTIMES_D(tmp[15]) ^ xTIMES_9(tmp[12]) ^ round_key[13];
    data[14] = xTIMES_E(tmp[14]) ^ xTIMES_B(tmp[15]) ^ xTIMES_D(tmp[12]) ^ xTIMES_9(tmp[13]) ^ round_key[14];
    data[15] = xTIMES_E(tmp[15]) ^ xTIMES_B(tmp[12]) ^ xTIMES_D(tmp[13]) ^ xTIMES_9(tmp[14]) ^ round_key[15];

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
void InvSubShiftARK(block data, block round_key){

    block tmp;
    // SubBytes, ShiftRows
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

void AES_Decrypt(block data, block key_schedule[11]){

    // Initial AddRoundKey
    uint32_t* dst = (uint32_t*)data;
    uint32_t* key = (uint32_t*)key_schedule[10];
    dst[0] ^= key[0];
    dst[1] ^= key[1];
    dst[2] ^= key[2];
    dst[3] ^= key[3];

    // Rounds 1-9
    InvSubShiftMixARK(data, key_schedule[9]);
    InvSubShiftMixARK(data, key_schedule[8]);
    InvSubShiftMixARK(data, key_schedule[7]);
    InvSubShiftMixARK(data, key_schedule[6]);
    InvSubShiftMixARK(data, key_schedule[5]);
    InvSubShiftMixARK(data, key_schedule[4]);
    InvSubShiftMixARK(data, key_schedule[3]);
    InvSubShiftMixARK(data, key_schedule[2]);
    InvSubShiftMixARK(data, key_schedule[1]);

    // Final Round
    InvSubShiftARK(data, key_schedule[0]);

}

block ciphertext = {
    0x7B, 0x0C, 0x78, 0x5E, 
    0x27, 0xE8, 0xAD, 0x3F, 
    0x82, 0x23, 0x20, 0x71, 
    0x04, 0x72, 0x5D, 0xD4
};

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
    /*printf("Ciphertext: ");
    for (int i = 0; i < 16; i++){
        if ((i == 3) || (i == 7) || (i == 11)){
            printf("%02X ", ciphertext[i]);
        } else{
            printf("%02X", ciphertext[i]);
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
    AES_Decrypt(ciphertext, in_key_schedule);
    /*printf("Plaintext: ");
    for (int i = 0; i < 16; i++){
        if ((i == 3) || (i == 7) || (i == 11)){
            printf("%02X ", ciphertext[i]);
        } else{
            printf("%02X", ciphertext[i]);
        }
    }
    printf("\n");*/
}