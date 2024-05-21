#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"


struct Chip8 *initialize_chip() {
    // struct Chip8 *chip = malloc(sizeof(struct Chip8));
    struct Chip8 *chip = calloc(1, sizeof(struct Chip8));
    chip->mem = malloc(sizeof(char) * 4096);
    chip->registers = malloc(sizeof(char) * 16);
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    
    chip->i = 0; // idk
    chip->pc = 0;
    

    load_fonts(chip, 100);

    return chip;
}

void de_initialize_chip(struct Chip8 *chip) {
    free(chip->registers);
    free(chip->mem);
    free(chip);
}

void load_fonts(struct Chip8 *chip, int address) {
    char fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0; i < 4 * 16; i++) {
        chip->mem[address + i] = fonts[i];
    }
}


void read_rom(struct Chip8 *chip8, char *filename, int mem_location) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("error reading rom\n");
        return;
    }
    char c;
    while( 1 == fread(&c, sizeof(char), 1, fp) ) {
        // printf("%x ", (unsigned char) c);
        chip8->mem[mem_location++] = c;
    }
}


unsigned short fetch_instruction(struct Chip8 *chip) {
    unsigned short instruction;
    instruction = chip->mem[chip->pc] << 4;
    instruction |= chip->mem[chip->pc + 1];
    chip->pc += 1;
    return instruction;
}


void execute_instruction(struct Chip8 *chip) {
    unsigned short instruction = fetch_instruction(chip);
    switch (instruction >> 6) {
        case 0x0:
            break;

        case 0x1:
        break;

        case 0x6:
        break;

        case 0x7:
        break;

        case 0xA:
        break;

        case 0xD:
        break;
    }
}