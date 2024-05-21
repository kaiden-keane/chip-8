#include "debug.h"
#include "stdio.h"

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