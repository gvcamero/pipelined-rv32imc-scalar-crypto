/*
AES-128 Decryption Program by Gabriel Camero
To be verified
*/

#include<stdio.h>
#include<stdint.h>
//#include<string.h>

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

/*
Organization of 'block' type:

In syntax:
{
A B C D (column)
E F G H (column)
I J K L (column)
M N O P (column)
}

Block Representation:

A E I M
B F J N
C G K O
D H L P

After Inv Shift Rows:

A E I M
N B F J
K O C G
H L P D

00 04 08 12
13 01 05 09
10 14 02 06
07 11 15 03

Hex Format: 0xDCBAHGFELKJIPONM

*/

// Global Variables:
block temp_block; // Provide a memory space for block copies to be used in InvMixColumns
uint32_t* temp_block_ptr = (uint32_t*)temp_block; // pointer for temp_block words
block key_schedule[10]; // Round keys array

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

// AES Round Functions

// InvShiftRows, InvSubBytes, and AddRoundKey
void InvShiftSubARK(block data, block round_key){

    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];
    
    data[0] = rsbox[temp_block[0]] ^ round_key[0];
    data[1] = rsbox[temp_block[13]] ^ round_key[1];
    data[2] = rsbox[temp_block[10]] ^ round_key[2];
    data[3] = rsbox[temp_block[7]] ^ round_key[3];

    data[4] = rsbox[temp_block[4]] ^ round_key[4];
    data[5] = rsbox[temp_block[1]] ^ round_key[5];
    data[6] = rsbox[temp_block[14]] ^ round_key[6];
    data[7] = rsbox[temp_block[11]] ^ round_key[7];

    data[8] = rsbox[temp_block[8]] ^ round_key[8];
    data[9] = rsbox[temp_block[5]] ^ round_key[9];
    data[10] = rsbox[temp_block[2]] ^ round_key[10];
    data[11] = rsbox[temp_block[15]] ^ round_key[11];

    data[12] = rsbox[temp_block[12]] ^ round_key[12];
    data[13] = rsbox[temp_block[9]] ^ round_key[13];
    data[14] = rsbox[temp_block[6]] ^ round_key[14];
    data[15] = rsbox[temp_block[3]] ^ round_key[15];

}

// InvMixColumns
void InvMixColumns(block data){

    // copy original block to save contents
    // memcpy(temp_block, data, 16);
    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];

    for(int c = 0; c < 4; c++){
        
        int i = c * 4;

        uint8_t b0 = temp_block[i];
        uint8_t b1 = temp_block[i + 1];
        uint8_t b2 = temp_block[i + 2];
        uint8_t b3 = temp_block[i + 3];

        data[i] = xTIMES_E(b0) ^ xTIMES_B(b1) ^ xTIMES_D(b2) ^ xTIMES_9(b3);
        data[i + 1] = xTIMES_9(b0) ^ xTIMES_E(b1) ^ xTIMES_B(b2) ^ xTIMES_D(b3);
        data[i + 2] = xTIMES_D(b0) ^ xTIMES_9(b1) ^ xTIMES_E(b2) ^ xTIMES_B(b3);
        data[i + 3] = xTIMES_B(b0) ^ xTIMES_D(b1) ^ xTIMES_9(b2) ^ xTIMES_E(b3);

    }

}

// AddRoundKey
void AddRoundKey(block data, block round_key){
    for (int i = 0; i < 16; i++){
        data[i] ^= round_key[i];
    }
}

// Key Schedule
void KeySchedule(block key){

    // key_schedule[0]
    uint32_t* nkey_col_ptr = (uint32_t*)&key_schedule[0][0];
    uint32_t* okey_col_ptr = (uint32_t*)&key[0];
    // RotWord and SubBytes
    key_schedule[0][0] = fsbox[key[13]];
    key_schedule[0][1] = fsbox[key[14]];
    key_schedule[0][2] = fsbox[key[15]];
    key_schedule[0][3] = fsbox[key[12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000001;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[1]
    nkey_col_ptr = (uint32_t*)&key_schedule[1][0];
    okey_col_ptr = (uint32_t*)&key_schedule[0][0];
    // RotWord and SubBytes
    key_schedule[1][0] = fsbox[key_schedule[0][13]];
    key_schedule[1][1] = fsbox[key_schedule[0][14]];
    key_schedule[1][2] = fsbox[key_schedule[0][15]];
    key_schedule[1][3] = fsbox[key_schedule[0][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000002;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[2]
    nkey_col_ptr = (uint32_t*)&key_schedule[2][0];
    okey_col_ptr = (uint32_t*)&key_schedule[1][0];
    // RotWord and SubBytes
    key_schedule[2][0] = fsbox[key_schedule[1][13]];
    key_schedule[2][1] = fsbox[key_schedule[1][14]];
    key_schedule[2][2] = fsbox[key_schedule[1][15]];
    key_schedule[2][3] = fsbox[key_schedule[1][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000004;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[3]
    nkey_col_ptr = (uint32_t*)&key_schedule[3][0];
    okey_col_ptr = (uint32_t*)&key_schedule[2][0];
    // RotWord and SubBytes
    key_schedule[3][0] = fsbox[key_schedule[2][13]];
    key_schedule[3][1] = fsbox[key_schedule[2][14]];
    key_schedule[3][2] = fsbox[key_schedule[2][15]];
    key_schedule[3][3] = fsbox[key_schedule[2][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000008;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[4]
    nkey_col_ptr = (uint32_t*)&key_schedule[4][0];
    okey_col_ptr = (uint32_t*)&key_schedule[3][0];
    // RotWord and SubBytes
    key_schedule[4][0] = fsbox[key_schedule[3][13]];
    key_schedule[4][1] = fsbox[key_schedule[3][14]];
    key_schedule[4][2] = fsbox[key_schedule[3][15]];
    key_schedule[4][3] = fsbox[key_schedule[3][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000010;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[5]
    nkey_col_ptr = (uint32_t*)&key_schedule[5][0];
    okey_col_ptr = (uint32_t*)&key_schedule[4][0];
    // RotWord and SubBytes
    key_schedule[5][0] = fsbox[key_schedule[4][13]];
    key_schedule[5][1] = fsbox[key_schedule[4][14]];
    key_schedule[5][2] = fsbox[key_schedule[4][15]];
    key_schedule[5][3] = fsbox[key_schedule[4][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000020;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[6]
    nkey_col_ptr = (uint32_t*)&key_schedule[6][0];
    okey_col_ptr = (uint32_t*)&key_schedule[5][0];
    // RotWord and SubBytes
    key_schedule[6][0] = fsbox[key_schedule[5][13]];
    key_schedule[6][1] = fsbox[key_schedule[5][14]];
    key_schedule[6][2] = fsbox[key_schedule[5][15]];
    key_schedule[6][3] = fsbox[key_schedule[5][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000040;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[7]
    nkey_col_ptr = (uint32_t*)&key_schedule[7][0];
    okey_col_ptr = (uint32_t*)&key_schedule[6][0];
    // RotWord and SubBytes
    key_schedule[7][0] = fsbox[key_schedule[6][13]];
    key_schedule[7][1] = fsbox[key_schedule[6][14]];
    key_schedule[7][2] = fsbox[key_schedule[6][15]];
    key_schedule[7][3] = fsbox[key_schedule[6][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000080;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[8]
    nkey_col_ptr = (uint32_t*)&key_schedule[8][0];
    okey_col_ptr = (uint32_t*)&key_schedule[7][0];
    // RotWord and SubBytes
    key_schedule[8][0] = fsbox[key_schedule[7][13]];
    key_schedule[8][1] = fsbox[key_schedule[7][14]];
    key_schedule[8][2] = fsbox[key_schedule[7][15]];
    key_schedule[8][3] = fsbox[key_schedule[7][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x0000001B;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];

    // key_schedule[9]
    nkey_col_ptr = (uint32_t*)&key_schedule[9][0];
    okey_col_ptr = (uint32_t*)&key_schedule[8][0];
    // RotWord and SubBytes
    key_schedule[9][0] = fsbox[key_schedule[8][13]];
    key_schedule[9][1] = fsbox[key_schedule[8][14]];
    key_schedule[9][2] = fsbox[key_schedule[8][15]];
    key_schedule[9][3] = fsbox[key_schedule[8][12]];
    // XORs
    nkey_col_ptr[0] = nkey_col_ptr[0] ^ okey_col_ptr[0] ^ 0x00000036;
    nkey_col_ptr[1] = nkey_col_ptr[0] ^ okey_col_ptr[1];
    nkey_col_ptr[2] = nkey_col_ptr[1] ^ okey_col_ptr[2];
    nkey_col_ptr[3] = nkey_col_ptr[2] ^ okey_col_ptr[3];
}

// Decrypt
void AES_Decrypt(block data, block key){

    // Key Schedule
    KeySchedule(key);

    // Initial AddRoundKey
    AddRoundKey(data, key_schedule[9]);

    // Rounds
    for(int i = 8; i >= 0; i--){
        InvShiftSubARK(data, key_schedule[i]);
        InvMixColumns(data);
    }

    // Last Round
    InvShiftSubARK(data, key);

}

/*void enc_round_mid(block data, block round_key){

    // 1st column

    // SubBytes
    data[0] = fsbox[data[0]];
    data[5] = fsbox[data[5]];
    data[10] = fsbox[data[10]];
    data[15] = fsbox[data[15]];

    // ShiftRows and MixColumns


}*/

// Inputs

block ciphertext = { // 7B0C785E 27E8AD3F 82232071 04725DD4 
    0x3A, 0xD7, 0x7B, 0xB4,
    0x0D, 0x7A, 0x36, 0x60,
    0xA8, 0x9E, 0xCA, 0xF3,
    0x24, 0x66, 0xEF, 0x97
};

block key = {
    0x2B, 0x7E, 0x15, 0x16,
    0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88,
    0x09, 0xCF, 0x4F, 0x3C
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
            printf("%02X ", key[i]);
        } else{
            printf("%02X", key[i]);
        }
    }
    printf("\n");*/
    AES_Decrypt(ciphertext, key);
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