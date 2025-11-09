/*
AES-128 Encryption Program by Gabriel Camero
Verified to be working correctly on all four official test vectors from NIPS
*/

//#include<stdio.h>
#include<stdint.h>
//#include<string.h>

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

After Shift Rows:

A E I M
F J N B
K O C G
P D H L

Hex Format: 0xDCBAHGFELKJIPONM

*/

// Global Variables:
block temp_block; // Provide a memory space for block copies to be used in MixColumns
uint32_t* temp_block_ptr = (uint32_t*)temp_block; // pointer for temp_block words
block key_schedule[10]; // Round keys array

// Function for Galois Field Multiplication by 2
uint8_t xTIMES(uint8_t byte){
    /*if (byte & 0x80){ // if MSB = 1
        return ((byte << 1) ^ (0x1B));
    }
    return (byte << 1);*/
    return (byte << 1) ^ ((byte & 0x80) ? 0x1B : 0x00);
}

// AES Round Functions

// SubBytes
void SubBytes(block data){
    for(int i = 0; i < 16; i++){
        data[i] = fsbox[data[i]];
    }
}

// ShiftRows and MixColumns and AddRoundKey
void ShiftMixARK(block data, block round_key){

    // copy original block to save contents
    // memcpy(temp_block, data, 16);
    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];

    for (int c = 0; c < 4; c++){

        int i = c * 4;

        uint8_t b0 = temp_block[i];
        uint8_t b1 = temp_block[(i + 5) & 0xF];
        uint8_t b2 = temp_block[(i + 10) & 0xF];
        uint8_t b3 = temp_block[(i + 15) & 0xF];

        data[i] = xTIMES(b0) ^ (b1 ^ xTIMES(b1)) ^ b2 ^ b3 ^ round_key[i];
        data[i + 1] = b0 ^ xTIMES(b1) ^ (b2 ^ xTIMES(b2)) ^ b3 ^ round_key[i + 1];
        data[i + 2] = b0 ^ b1 ^ xTIMES(b2) ^ (b3 ^ xTIMES(b3)) ^ round_key[i + 2];
        data[i + 3] = (b0 ^ xTIMES(b0)) ^ b1 ^ b2 ^ xTIMES(b3) ^ round_key[i + 3];
    }

    /*
    // 1st col [0, 5, 10, 15]
    data[0] = (xTIMES(temp_block[0]) ^ (temp_block[5] ^ xTIMES(temp_block[5])) ^ temp_block[10] ^ temp_block[15])^round_key[0];
    data[1] = (temp_block[0] ^ xTIMES(temp_block[5]) ^ (temp_block[10] ^ xTIMES(temp_block[10])) ^ temp_block[15])^round_key[1];
    data[2] = (temp_block[0] ^ temp_block[5] ^ xTIMES(temp_block[10]) ^ (temp_block[15] ^ xTIMES(temp_block[15])))^round_key[2];
    data[3] = ((temp_block[0] ^ xTIMES(temp_block[0])) ^ temp_block[5] ^ temp_block[10] ^ xTIMES(temp_block[15]))^round_key[3];

    // 2nd col [4, 9, 14, 3]
    data[4] = (xTIMES(temp_block[4]) ^ (temp_block[9] ^ xTIMES(temp_block[9])) ^ temp_block[14] ^ temp_block[3])^round_key[4];
    data[5] = (temp_block[4] ^ xTIMES(temp_block[9]) ^ (temp_block[14] ^ xTIMES(temp_block[14])) ^ temp_block[3])^round_key[5];
    data[6] = (temp_block[4] ^ temp_block[9] ^ xTIMES(temp_block[14]) ^ (temp_block[3] ^ xTIMES(temp_block[3])))^round_key[6];
    data[7] = ((temp_block[4] ^ xTIMES(temp_block[4])) ^ temp_block[9] ^ temp_block[14] ^ xTIMES(temp_block[3]))^round_key[7];

    // 3rd col [8, 13, 2, 7]
    data[8] = (xTIMES(temp_block[8]) ^ (temp_block[13] ^ xTIMES(temp_block[13])) ^ temp_block[2] ^ temp_block[7])^round_key[8];
    data[9] = (temp_block[8] ^ xTIMES(temp_block[13]) ^ (temp_block[2] ^ xTIMES(temp_block[2])) ^ temp_block[7])^round_key[9];
    data[10] = (temp_block[8] ^ temp_block[13] ^ xTIMES(temp_block[2]) ^ (temp_block[7] ^ xTIMES(temp_block[7])))^round_key[10];
    data[11] = ((temp_block[8] ^ xTIMES(temp_block[8])) ^ temp_block[13] ^ temp_block[2] ^ xTIMES(temp_block[7]))^round_key[11];

    // 4th col [12, 1, 6, 11]
    data[12] = (xTIMES(temp_block[12]) ^ (temp_block[1] ^ xTIMES(temp_block[1])) ^ temp_block[6] ^ temp_block[11])^round_key[12];
    data[13] = (temp_block[12] ^ xTIMES(temp_block[1]) ^ (temp_block[6] ^ xTIMES(temp_block[6])) ^ temp_block[11])^round_key[13];
    data[14] = (temp_block[12] ^ temp_block[1] ^ xTIMES(temp_block[6]) ^ (temp_block[11] ^ xTIMES(temp_block[11])))^round_key[14];
    data[15] = ((temp_block[12] ^ xTIMES(temp_block[12])) ^ temp_block[1] ^ temp_block[6] ^ xTIMES(temp_block[11]))^round_key[15];
    */

}

// ShiftRows and AddRoundKey
void ShiftRowsARK(block data){
    
    // copy original block to save contents
    //memcpy(temp_block, data, 16);
    uint32_t* data_ptr = (uint32_t*)data;
    temp_block_ptr[0] = data_ptr[0];
    temp_block_ptr[1] = data_ptr[1];
    temp_block_ptr[2] = data_ptr[2];
    temp_block_ptr[3] = data_ptr[3];

    data[0] ^= key_schedule[9][0];
    data[1] = temp_block[5] ^ key_schedule[9][1];
    data[2] = temp_block[10] ^ key_schedule[9][2];
    data[3] = temp_block[15] ^ key_schedule[9][3];

    data[4] ^= key_schedule[9][4];
    data[5] = temp_block[9] ^ key_schedule[9][5];
    data[6] = temp_block[14] ^ key_schedule[9][6];
    data[7] = temp_block[3] ^ key_schedule[9][7];

    data[8] ^= key_schedule[9][8];
    data[9] = temp_block[13] ^ key_schedule[9][9];
    data[10] = temp_block[2] ^ key_schedule[9][10];
    data[11] = temp_block[7] ^ key_schedule[9][11];

    data[12] ^= key_schedule[9][12];
    data[13] = temp_block[1] ^ key_schedule[9][13];
    data[14] = temp_block[6] ^ key_schedule[9][14];
    data[15] = temp_block[11] ^ key_schedule[9][15];

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

// Encrypt
void AES_Encrypt(block data, block key){

    // Key Schedule
    KeySchedule(key);

    // Initial AddRoundKey
    AddRoundKey(data, key);

    // Rounds
    for(int i = 0; i < 9; i++){
        SubBytes(data);
        ShiftMixARK(data, key_schedule[i]);
    }

    // Last Round
    SubBytes(data);
    ShiftRowsARK(data);

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

block plaintext = {
    0xF6, 0x9F, 0x24, 0x45,
    0xDF, 0x4F, 0x9B, 0x17,
    0xAD, 0x2B, 0x41, 0x7B,
    0xE6, 0x6C, 0x37, 0x10
};

block key = {
    0x2B, 0x7E, 0x15, 0x16,
    0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88,
    0x09, 0xCF, 0x4F, 0x3C
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
            printf("%02X ", key[i]);
        } else{
            printf("%02X", key[i]);
        }
    }
    printf("\n");*/
    AES_Encrypt(plaintext, key);
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