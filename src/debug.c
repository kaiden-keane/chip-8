#include "debug.h"

// prints all memory
void print_memory(struct Chip8 *chip) {
    // todo make it print in neat collumns
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%x ", (unsigned char) chip->mem[32*i + j]);
        }
        printf("\n");
    }
}

// thanks stack overflow
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void print_instruction(unsigned short instruction) {
    unsigned char a = (instruction >> 12) & 0xF;
    unsigned char b = (instruction >> 8) & 0xF;
    unsigned char c = (instruction >> 4) & 0xF;
    unsigned char d = (instruction) & 0xF;
    printf("%x%x%x%x\n", a, b, c, d);
}

void print_registers(struct Chip8 *chip) {
    for (int i = 0; i < 16; i++) {
        printf("%d ", chip->registers[i]);
    }
    printf("\n");
}