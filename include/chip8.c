#include "stdio.h"
#include "chip8.h"

int read_rom(char *filename, int mem_location) {
    FILE *fp = fopen(filename, "rb");
    char c;
    int current_location = mem_location;
    while(c = fgetc(fp)) {
        if (c != EOF) {
            return 0;
        }
        else chip8.mem[current_location] = c;


    }
}

unsigned short fetch_instruction() {
    unsigned short instruction;
    instruction = chip8.mem[chip8.pc] << 4;
    instruction |= chip8.mem[chip8.pc + 1];
    chip8.pc += 1;
    return instruction;
}