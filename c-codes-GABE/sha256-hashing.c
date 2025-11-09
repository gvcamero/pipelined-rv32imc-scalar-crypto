#include<stdint.h>
#include<stdio.h>
#include<string.h>

// K words
static const uint32_t k_words[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// sum0
uint32_t sum0(const uint32_t data){
    return ((data >> 2) | (data << 30)) ^ ((data >> 13) | (data << 19)) ^ ((data >> 22) | (data << 10));
}

// sum1
uint32_t sum1(const uint32_t data){
    return ((data >> 6) | (data << 26)) ^ ((data >> 11) | (data << 21)) ^ ((data >> 25) | (data << 7));
}

// sig0
uint32_t sig0(const uint32_t data){
    return ((data >> 7) | (data << 25)) ^ ((data >> 18) | (data << 14)) ^ (data >> 3);
}

// sig1
uint32_t sig1(const uint32_t data){
    return ((data >> 17) | (data << 15)) ^ ((data >> 19) | (data << 13)) ^ (data >> 10);
}

// Ch
uint32_t Ch(const uint32_t x, const uint32_t y, const uint32_t z){
    return (x & y) ^ (~x & z);
}

// Maj
uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z){
    return (x & y) ^ (x & z) ^ (y & z);
}

/* OTHER FUNCTIONS */

// byte swap
uint32_t bswap32(const uint32_t data){
    return ((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) | ((data & 0x00FF0000) >> 8) | ((data & 0xFF000000) >> 24);
}

/* main hash function */
void sha256_hash(const uint8_t* data, size_t num_blocks, uint32_t result[8]){

    // Initialized hash values
    result[0] = 0x6A09E667;
    result[1] = 0xBB67AE85;
    result[2] = 0x3C6EF372;
    result[3] = 0xA54FF53A;
    result[4] = 0x510E527F;
    result[5] = 0x9B05688C;
    result[6] = 0x1F83D9AB;
    result[7] = 0x5BE0CD19;

    // Initialize message schedule array
    uint32_t msg_sch[64];

    // block loop
    const uint32_t* block_ptr;
    for(size_t i = 0; i < num_blocks; i++){

        // block pointer
        block_ptr = (uint32_t*)data + (i * 16);
        
        // Prepare the message schedule
        for (int t = 0; t < 64; t++){
            if(t < 16){
                msg_sch[t] = bswap32(block_ptr[t]);
            }
            else{
                msg_sch[t] = sig1(msg_sch[t-2]) + msg_sch[t-7] + sig0(msg_sch[t-15]) + msg_sch[t-16];
            }
        }

        // Eight working variables
        uint32_t a = result[0];
        uint32_t b = result[1];
        uint32_t c = result[2];
        uint32_t d = result[3];
        uint32_t e = result[4];
        uint32_t f = result[5];
        uint32_t g = result[6];
        uint32_t h = result[7];

        // Loop
        for(int t = 0; t < 64; t++){
            uint32_t T1 = h + sum1(e) + Ch(e, f, g) + k_words[t] + msg_sch[t];
            uint32_t T2 = sum0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        // Compute intermediate hash value
        result[0] = a + result[0];
        result[1] = b + result[1];
        result[2] = c + result[2];
        result[3] = d + result[3];
        result[4] = e + result[4];
        result[5] = f + result[5];
        result[6] = g + result[6];
        result[7] = h + result[7];

    }

}

// input
uint8_t input_array[] = {
    0x53, 0x6F, 0x20, 0x69, 0x66, 0x20, 0x79, 0x6F, 
    0x75, 0x20, 0x63, 0x61, 0x72, 0x65, 0x20, 0x74, 
    0x6F, 0x20, 0x66, 0x69, 0x6E, 0x64, 0x20, 0x6D, 
    0x65, 0x2C, 0x0A, 0x6C, 0x6F, 0x6F, 0x6B, 0x20, 
    0x74, 0x6F, 0x20, 0x74, 0x68, 0x65, 0x20, 0x77, 
    0x65, 0x73, 0x74, 0x65, 0x72, 0x6E, 0x20, 0x73, 
    0x6B, 0x79, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x90
};

// num_blocks
size_t num_blocks = sizeof(input_array) / 64;

// result array
uint32_t result[8];

int main(){
    sha256_hash(input_array, num_blocks, result);
    for(int i = 0; i < 8; i++){
        printf("%08X", result[i]);
    }
}