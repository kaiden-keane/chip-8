#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "debug.h"
#include "graphics.h"
#include <string.h> // for memset

struct Chip8 *initialize_chip() {
    struct Chip8 *chip = calloc(1, sizeof(struct Chip8));
    chip->mem = calloc(4096, sizeof(char));
    chip->registers = malloc(sizeof(char) * 16);

    memset(chip->display, 0, sizeof(chip->display));
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    
    chip->i = 0; // not needed just might aswell

    return chip;
}

void deinitialize_chip(struct Chip8 *chip) {
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
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("error reading rom\n");
        return;
     }
    unsigned char c;
    while( 1 == fread(&c, sizeof(char), 1, fp) ) {
        chip8->mem[mem_location++] = c;
    }
}


unsigned short fetch_instruction(struct Chip8 *chip, unsigned short mem_location) {
    return (short)(((short)chip->mem[mem_location]) << 8) | chip->mem[mem_location + 1];
}


void execute_instruction(struct Chip8 *chip) {
    unsigned short instruction = fetch_instruction(chip, chip->pc);

    chip->pc += 2;

    unsigned char nibble = (instruction >> (16-4)) & 0xF;
    
    switch (nibble) {
        case 0x0: // clear screen
            if ( (instruction & 0xFF) == 0xE0) { // clear screen
                clear_screen(&chip->screen);
                render_screen(&chip->screen);
            }
        break;

        case 0x1: // jump (set pc to nnn)
            chip->pc = instruction & 0xFFF;
        break;
        
        case 0x2: // 
            
        break;
        
        case 0x3: // 
            
        break;
        
        case 0x4: // 
            
        break;
        
        case 0x5: // 
            
        break;
        
        case 0x6: // set registers
            chip->registers[(instruction >> 8) & 0xF] = instruction & 0xFF;
        break;

        case 0x7: // add registers
            chip->registers[(instruction >> 8) & 0xF] += instruction & 0xFF;
        break;

        case 0x8: // 
            
        break;
        
        case 0x9: // 
            
        break;
        

        case 0xA: // set index
            chip->i = instruction & 0xFFF;
        break;

        case 0xB: // 
            
        break;
        
        case 0xC: // 
            
        break;
        
        case 0xD: // display
            draw_sprite(chip, instruction);
            draw_display(chip);
        break;

        case 0xE: // 
            
        break;
        
        case 0xF: // 
            
        break;
        
    }
}