#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "debug.h"
#include "graphics.h"
#include <string.h> // for memset

struct Chip8 *initialize_chip() {
    // struct Chip8 *chip = malloc(sizeof(struct Chip8));
    struct Chip8 *chip = calloc(1, sizeof(struct Chip8));
    chip->mem = calloc(4096, sizeof(char));
    chip->registers = malloc(sizeof(char) * 16);

    memset(chip->display, 0, sizeof(chip->display));
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    
    chip->i = 0; // idk
    chip->pc = 0; // ?

    return chip;
}

void de_initialize_chip(struct Chip8 *chip) {
    free(chip->registers);
    free(chip->mem);
    free(chip);
}

void load_fonts(struct Chip8 *chip, int address) {
    unsigned char fonts[] = {
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
    printf("hrege\n");
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("error reading rom\n");
        return;
     }
    unsigned char c;
    while( 1 == fread(&c, sizeof(char), 1, fp) ) {
        printf("%x ", (unsigned char) c);
        chip8->mem[mem_location++] = c;
    }
}


unsigned short fetch_instruction(struct Chip8 *chip, unsigned short mem_location) {
    // unsigned short instruction;
    // printf("val in mem: %x%x:\n", chip->mem[mem_location], chip->mem[mem_location + 1]);
    // instruction = (unsigned short)chip->mem[mem_location] << 8;
    // instruction |= (unsigned char) chip->mem[mem_location + 1];

    // return instruction;
    return (short)(((short)chip->mem[mem_location]) << 8) | chip->mem[mem_location + 1];
}


void execute_instruction(struct Chip8 *chip) {
    unsigned short instruction = fetch_instruction(chip, chip->pc);

    printf("current instruction: "); print_instruction(instruction);

    chip->pc += 2;
    /*
    ((instruction >> (16-4)) & 0xF)
    ((instruction >> (16-8)) & 0xF)
    ((instruction >> (16-12)) & 0xF)
    (instruction & 0b1111)
    */
    unsigned char nibble = (instruction >> (16-4)) & 0xF;
    // printf("\tcurrent nibble: %x\n", nibble);
    // first nibble
    
    switch (nibble) {
        case 0x0: // works
            printf("0x0\n");
            if ( (instruction & 0xFF) == 0xE0) { // clear screen
                clear_screen(&chip->screen);
                render_screen(&chip->screen);
            }
        break;

        case 0x1: // works (1228 went back to itself)
            printf("0x1\n");
            chip->pc = instruction & 0xFFF;
        break;

        case 0x6: // works
            printf("0x6\n");
            chip->registers[(instruction >> 8) & 0xF] = instruction & 0xFF;
        break;

        case 0x7: // works
            printf("0x7\n");
            chip->registers[(instruction >> 8) & 0xF] += instruction & 0xFF;
        break;

        case 0xA: // works
            printf("0xA\n");
            chip->i = instruction & 0xFFF;
        break;

        case 0xD: // no :(
            printf("0xD\n");
            draw_sprite(chip, chip->registers[(instruction >> 8) & 0xF], 
                    chip->registers[(instruction >> 4) & 0xF], instruction & 0xF);
            draw_display(chip);
        break;
    }
}