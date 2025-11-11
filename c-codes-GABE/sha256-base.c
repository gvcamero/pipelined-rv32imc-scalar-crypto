#include<stdint.h>
#include<stdio.h>
#include<string.h>

// Global input
char message[] = "Matcha is a finely ground powder made from specially grown and processed green tea leaves, celebrated for its vibrant green color, distinct earthy flavor, and numerous health benefits. Unlike regular green tea, where the leaves are steeped and then discarded, matcha involves consuming the entire leaf in powdered form, which makes it far richer in antioxidants, amino acids, and nutrients such as catechins, chlorophyll, and L-theanine. Its cultivation requires careful shading of the tea plants for several weeks before harvest, a process that boosts chlorophyll production and deepens the tea's vivid hue while enhancing its natural sweetness and umami taste. After harvesting, the leaves are steamed to prevent oxidation, dried, and stone-ground into an ultra-fine powder using traditional granite mills, resulting in a texture as smooth as talcum powder.";

// Global output
uint32_t hash_result[8];

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

// byte swap
uint32_t bswap32(const uint32_t data){
    return  (data << 24)           | 
            ((data & 0xFF00) << 8) |
            ((data >> 8) & 0xFF00) | 
            (data >> 24);
}

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
    return (x & (y ^ z)) ^ z;
}

// Maj
uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z){
    return (x & y) | (z & (x | y));
}

void SHA256_Hash(char* data){

    // ----------- PRE-PROCESSING ------------------------------------------------------------

    // string length (number of bytes of string)
    size_t data_len = strlen(data);

    // Find the number of zero bytes to be appended (excluding 0x80)
    size_t zbytes = (64 - ((data_len + 9) & 63)) & 63;

    // Size of padded message in bytes
    size_t padded_bytes = data_len + 9 + zbytes;
    // No. of blocks
    size_t num_blocks = padded_bytes >> 6;

    // Initialize padded message array
    uint8_t padded_message[padded_bytes];

    // Copy original data to padded message array
    memcpy(padded_message, data, data_len);

    // pad 0x00 bytes
    memset(padded_message + data_len, 0x00, zbytes + 1);
    // pad 0x80
    padded_message[data_len] = 0x80;

    // pad 8-byte length
    uint64_t bit_len = (uint64_t)data_len << 3;
    padded_message[padded_bytes - 8] = (bit_len >> 56) & 0xFF;
    padded_message[padded_bytes - 7] = (bit_len >> 48) & 0xFF;
    padded_message[padded_bytes - 6] = (bit_len >> 40) & 0xFF;
    padded_message[padded_bytes - 5] = (bit_len >> 32) & 0xFF;
    padded_message[padded_bytes - 4] = (bit_len >> 24) & 0xFF;
    padded_message[padded_bytes - 3] = (bit_len >> 16) & 0xFF;
    padded_message[padded_bytes - 2] = (bit_len >> 8) & 0xFF;
    padded_message[padded_bytes - 1] = bit_len & 0xFF;

    // OUTPUT PRINTING FOR DEBUGGING PURPOSES
    /*printf("uint8_t input_array[] = {\n    ");
    for (size_t i = 0; i < padded_bytes; i++) {
        printf("0x%02X", padded_message[i]);
        if (i != padded_bytes - 1) printf(", ");
    
        // Add line break every 8 bytes
        if ((i + 1) % 8 == 0 && i != padded_bytes - 1) {
            printf("\n    ");
        }
    }
    printf("\n};\n");

    printf("Number of blocks: %zu\n", num_blocks);*/

    // ----------- HASHING -----------------------------------------------------------------

    // Initialized hash values
    hash_result[0] = 0x6A09E667;
    hash_result[1] = 0xBB67AE85;
    hash_result[2] = 0x3C6EF372;
    hash_result[3] = 0xA54FF53A;
    hash_result[4] = 0x510E527F;
    hash_result[5] = 0x9B05688C;
    hash_result[6] = 0x1F83D9AB;
    hash_result[7] = 0x5BE0CD19;

    // Initialize message schedule array
    uint32_t msg_sch[64];

    // Pointer to the 512-bit blocks array
    uint32_t* block_ptr;
    // Main hash loop that iterates across all blocks
    for(size_t i = 0; i < num_blocks; i++){

        block_ptr = (uint32_t*)padded_message + (i * 16);

        // Prepare message schedule: first 16 words
        msg_sch[0] = bswap32(block_ptr[0]);
        msg_sch[1] = bswap32(block_ptr[1]);
        msg_sch[2] = bswap32(block_ptr[2]);
        msg_sch[3] = bswap32(block_ptr[3]);
        msg_sch[4] = bswap32(block_ptr[4]);
        msg_sch[5] = bswap32(block_ptr[5]);
        msg_sch[6] = bswap32(block_ptr[6]);
        msg_sch[7] = bswap32(block_ptr[7]);
        msg_sch[8] = bswap32(block_ptr[8]);
        msg_sch[9] = bswap32(block_ptr[9]);
        msg_sch[10] = bswap32(block_ptr[10]);
        msg_sch[11] = bswap32(block_ptr[11]);
        msg_sch[12] = bswap32(block_ptr[12]);
        msg_sch[13] = bswap32(block_ptr[13]);
        msg_sch[14] = bswap32(block_ptr[14]);
        msg_sch[15] = bswap32(block_ptr[15]);

        // Rest of message schedule
        for(int j = 16; j < 64; j++){
            msg_sch[j] = sig1(msg_sch[j-2]) + msg_sch[j-7] + sig0(msg_sch[j-15]) + msg_sch[j-16];
        }

        // Eight working variables
        uint32_t a = hash_result[0];
        uint32_t b = hash_result[1];
        uint32_t c = hash_result[2];
        uint32_t d = hash_result[3];
        uint32_t e = hash_result[4];
        uint32_t f = hash_result[5];
        uint32_t g = hash_result[6];
        uint32_t h = hash_result[7];

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
        hash_result[0] = a + hash_result[0];
        hash_result[1] = b + hash_result[1];
        hash_result[2] = c + hash_result[2];
        hash_result[3] = d + hash_result[3];
        hash_result[4] = e + hash_result[4];
        hash_result[5] = f + hash_result[5];
        hash_result[6] = g + hash_result[6];
        hash_result[7] = h + hash_result[7];
    }

}

int main(){
    SHA256_Hash(message);
    // UNCOMMENT IF NEED TO PRINT OUTPUT
    /*for(int i = 0; i < 8; i++){
        printf("%08X", hash_result[i]);
    }*/
}