#include<stdint.h>
#include<stdio.h>
#include<string.h>

// Global input
char message[] = "So if you care to find me,\nlook to the western sky";

void Padding(char* data){

    // Get input size + 1 + 64
    size_t partial_size = (strlen(data) * 8) + 1 + 64; // no. of bits

    // Find number of zero bits to append
    size_t zbits = (((partial_size + 511) / 512) * 512) - partial_size;

    // Find the number of zero bytes to be appended (excluding 0x80)
    size_t zbytes = (zbits - 7) / 8;

    // Size of padded message in bytes
    size_t padded_bytes = strlen(data) + 1 + 8 + zbytes;

    uint8_t padded_message[padded_bytes];

    memcpy(padded_message, data, strlen(data));

    // pad 0x80
    padded_message[strlen(data)] = 0x80;

    // pad 0x00 bytes
    memset(padded_message + strlen(data) + 1, 0x00, zbytes);

    // pad 8-byte length
    uint64_t bit_len = strlen(data) * 8;
    for(int i = 0; i < 8; i++){
        padded_message[padded_bytes - 8 + i] = (bit_len >> (56 - i * 8)) & 0xFF;
    }

    //printf("%zu\n", strlen(data));
    //printf("%zu\n", zbits);

    //for (int i = 0; i < padded_bytes; i++){
    //    printf("%02X ", padded_message[i]);
    //}
    //printf("\n");
    //printf("Number of zero bytes appended: %zu", zbytes);

    printf("uint8_t input_array[] = {\n    ");
    for (size_t i = 0; i < padded_bytes; i++) {
        printf("0x%02X", padded_message[i]);
        if (i != padded_bytes - 1) printf(", ");
    
        // Add line break every 8 bytes
        if ((i + 1) % 8 == 0 && i != padded_bytes - 1) {
            printf("\n    ");
        }
    }
    printf("\n};\n");

}

int main(){
    Padding(message);
}