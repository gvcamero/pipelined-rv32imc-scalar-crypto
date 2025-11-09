/*
KeySchedule program by Gabriel Camero
To be compiled with extensions
*/

#include<stdint.h>

// create new type 'block' which is an array of 16 bytes
typedef uint8_t block[16];

// Global variables
block key_schedule[10]; // Round keys array

// Key Schedule
void KeySchedule(block key){

    uint32_t* key_col = (uint32_t*)key;
    uint32_t* key_sched_col = (uint32_t*)key_schedule[0];

    // Key 1
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x01\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_col[3]), "r"(key_col[0]), "r"(&key_sched_col[0]), "r"(key_col[1]), "r"(&key_sched_col[1]), "r"(key_col[2]), "r"(&key_sched_col[2]), "r"(&key_sched_col[3])
                : "t1", "t0", "memory");
    
    // Key 2
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x02\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[3]), "r"(key_sched_col[0]), "r"(&key_sched_col[4]), "r"(key_sched_col[1]), "r"(&key_sched_col[5]), "r"(key_sched_col[2]), "r"(&key_sched_col[6]), "r"(&key_sched_col[7])
                : "t1", "t0", "memory");

    // Key 3
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x04\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[7]), "r"(key_sched_col[4]), "r"(&key_sched_col[8]), "r"(key_sched_col[5]), "r"(&key_sched_col[9]), "r"(key_sched_col[6]), "r"(&key_sched_col[10]), "r"(&key_sched_col[11])
                : "t1", "t0", "memory");

    // Key 4
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x08\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[11]), "r"(key_sched_col[8]), "r"(&key_sched_col[12]), "r"(key_sched_col[9]), "r"(&key_sched_col[13]), "r"(key_sched_col[10]), "r"(&key_sched_col[14]), "r"(&key_sched_col[15])
                : "t1", "t0", "memory");

    // Key 5
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x10\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[15]), "r"(key_sched_col[12]), "r"(&key_sched_col[16]), "r"(key_sched_col[13]), "r"(&key_sched_col[17]), "r"(key_sched_col[14]), "r"(&key_sched_col[18]), "r"(&key_sched_col[19])
                : "t1", "t0", "memory");

    // Key 6
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x20\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[19]), "r"(key_sched_col[16]), "r"(&key_sched_col[20]), "r"(key_sched_col[17]), "r"(&key_sched_col[21]), "r"(key_sched_col[18]), "r"(&key_sched_col[22]), "r"(&key_sched_col[23])
                : "t1", "t0", "memory");

    // Key 7
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x40\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[23]), "r"(key_sched_col[20]), "r"(&key_sched_col[24]), "r"(key_sched_col[21]), "r"(&key_sched_col[25]), "r"(key_sched_col[22]), "r"(&key_sched_col[26]), "r"(&key_sched_col[27])
                : "t1", "t0", "memory");

    // Key 8
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x80\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[27]), "r"(key_sched_col[24]), "r"(&key_sched_col[28]), "r"(key_sched_col[25]), "r"(&key_sched_col[29]), "r"(key_sched_col[26]), "r"(&key_sched_col[30]), "r"(&key_sched_col[31])
                : "t1", "t0", "memory");

    // Key 9
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x1B\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[31]), "r"(key_sched_col[28]), "r"(&key_sched_col[32]), "r"(key_sched_col[29]), "r"(&key_sched_col[33]), "r"(key_sched_col[30]), "r"(&key_sched_col[34]), "r"(&key_sched_col[35])
                : "t1", "t0", "memory");

    // Key 10
    asm volatile("rori t1, %0, 8\n\t"
                 "aes32esi t0, zero, t1, 0\n\t"
                 "aes32esi t0, t0, t1, 1\n\t"
                 "aes32esi t0, t0, t1, 2\n\t"
                 "aes32esi t0, t0, t1, 3\n\t"
                 "xor t0, t0, %1\n\t"
                 "xori t0, t0, 0x36\n\t"
                 "sw t0, 0(%2)\n\t"
                 "xor t0, t0, %3\n\t"
                 "sw t0, 0(%4)\n\t"
                 "xor t0, t0, %5\n\t"
                 "sw t0, 0(%6)\n\t"
                 "xor t0, t0, %0\n\t"
                 "sw t0, 0(%7)"
                : 
                : "r"(key_sched_col[35]), "r"(key_sched_col[32]), "r"(&key_sched_col[36]), "r"(key_sched_col[33]), "r"(&key_sched_col[37]), "r"(key_sched_col[34]), "r"(&key_sched_col[38]), "r"(&key_sched_col[39])
                : "t1", "t0", "memory");
    
}

// input key
block in_key = {
    0x2B, 0x7E, 0x15, 0x16,
    0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88,
    0x09, 0xCF, 0x4F, 0x3C
};

int main(){

    KeySchedule(in_key);

}