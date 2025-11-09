/*
InvMixColumns Program by Gabriel Camero
To be compiled with base RV32 only
*/

//#include<stdio.h>
#include<stdint.h>

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

void InvMixColumns(block data){

    block tmp;
    uint32_t *data_col = (uint32_t*)data;
    uint32_t *tmp_col = (uint32_t*)tmp;
    tmp_col[0] = data_col[0];
    tmp_col[1] = data_col[1];
    tmp_col[2] = data_col[2];
    tmp_col[3] = data_col[3];

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

// Input key schedule:

block key_schedule[9] = {
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
    }
};

/*
// ---------- NEW PRINTING FUNCTIONS BELOW ----------
// Optional, used for testing purposes. Uncommented only when needed

// Print results in C array declaration format
void print_c_array_format(block keys[], int count) {
    printf("block invmix_key_schedule[%d] = {\n", count);
    for (int b = 0; b < count; b++) {
        printf("    {\n        ");
        for (int i = 0; i < 16; i++) {
            printf("0x%02X", keys[b][i]);
            if (i < 15) printf(", ");
            if ((i + 1) % 4 == 0 && i != 15) printf("\n        ");
        }
        printf("\n    }%s\n", (b < count - 1) ? "," : "");
    }
    printf("};\n");
}

// Print results in memory dump format
void print_memory_dump(block keys[], int count) {
    for (int b = 0; b < count; b++) {
        for (int i = 0; i < 16; i += 4) {
            printf("%02X%02X%02X%02X\n",
                   keys[b][i], keys[b][i + 1],
                   keys[b][i + 2], keys[b][i + 3]);
        }
    }
}
*/

int main(){
    InvMixColumns(key_schedule[0]);
    InvMixColumns(key_schedule[1]);
    InvMixColumns(key_schedule[2]);
    InvMixColumns(key_schedule[3]);
    InvMixColumns(key_schedule[4]);
    InvMixColumns(key_schedule[5]);
    InvMixColumns(key_schedule[6]);
    InvMixColumns(key_schedule[7]);
    InvMixColumns(key_schedule[8]);

    /*
    // ----- Comment out the following when not needed -----
    int choice;
    printf("Choose output format:\n1. C array format\n2. Memory dump format\n> ");
    scanf("%d", &choice);

    if (choice == 1)
        print_c_array_format(key_schedule, 9);
    else if (choice == 2)
        print_memory_dump(key_schedule, 9);
    else
        printf("Invalid choice.\n");
    // ------------------------------------------------------
    */

}