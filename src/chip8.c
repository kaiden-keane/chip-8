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
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    
    chip->i = 0; // not needed just might aswell

    chip->sp = 0;
    chip->pc = ENTRY_POINT;

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
            if ((instruction & 0xFF) == 0xE0) { // clear screen
                clear_screen(&chip->screen->texture);
                render_screen(chip->screen);
            }
            else if ((instruction & 0xFF) == 0xEE) {
                chip->pc = chip->stack[chip->sp];
                chip->sp -= 1;
            }
        break;

        case 0x1: // jump (set pc to nnn)
            chip->pc = instruction & 0xFFF;
        break;
        
        case 0x2: // call
            chip->sp += 1;
            chip->stack[chip->sp - 1] = chip->pc; // to account for += 1
            chip->pc = instruction & 0xFFF; 
        break;
        
        case 0x3: // skip if Vx = kk
            if (chip->registers[(instruction >> 8) & 0xF] == (instruction & 0xF)) {
                chip->pc += 2;
            }
        break;
        
        case 0x4: // skip if not equal
            if (chip->registers[(instruction >> 8) & 0xF] != (instruction & 0xF)) {
                chip->pc += 2;
            }
        break;
        
        case 0x5: // skip if Vx = Vy
            if (chip->registers[(instruction >> 8) & 0xF] == chip->registers[(instruction >> 4) & 0xF]) {
                chip->pc += 2;
            }
        break;
        
        case 0x6: // set registers
            chip->registers[(instruction >> 8) & 0xF] = (instruction & 0xFF);
        break;

        case 0x7: // add registers
            chip->registers[(instruction >> 8) & 0xF] += (instruction & 0xFF);
        break;

        case 0x8: 
            switch (instruction & 0xF) {
                case 0: // set Vx = Vy (8xy0)
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 4) & 0xF];
                break;

                case 1: // bitwise OR
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 8) & 0xF] | chip->registers[(instruction >> 4) & 0xF];
                break;

                case 2: // bitwise AND
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 8) & 0xF] & chip->registers[(instruction >> 4) & 0xF];
                break;

                case 3: // bitwise XOR
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 8) & 0xF] ^ chip->registers[(instruction >> 4) & 0xF];
                break;

                case 4: // addition
                    chip->registers[0xF] = 0;
                    unsigned int result = chip->registers[(instruction >> 8) & 0xF] + chip->registers[(instruction >> 4) & 0xF];
                    if (result > 255) {
                        chip->registers[0xF] = 1;
                    }
                    chip->registers[(instruction >> 8) & 0xF] = result;
                break;

                case 5: // subtraction
                    if (chip->registers[(instruction >> 8) & 0xF] > chip->registers[(instruction >> 4) & 0xF]) {
                        chip->registers[0xF] = 1;
                    } else {
                        chip->registers[0xF] = 0;
                    }
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 8) & 0xF] - chip->registers[(instruction >> 4) & 0xF];
                break;

                case 6:
                    if ((instruction & 0xF) == 1) {
                        chip->registers[0xF] = 1;
                    } else {
                        chip->registers[0xF] = 0;
                    }
                    chip->registers[(instruction >> 8) & 0xF] /= 2; // equivalent to x >> 1
                break;

                case 7:
                    if (chip->registers[(instruction >> 8) & 0xF] < chip->registers[(instruction >> 4) & 0xF]) {
                        chip->registers[0xF] = 1;
                    } else {
                        chip->registers[0xF] = 0;
                    }
                    chip->registers[(instruction >> 8) & 0xF] = chip->registers[(instruction >> 4) & 0xF] - chip->registers[(instruction >> 8) & 0xF];
                break;

                case 0xE:
                   if ((chip->registers[(instruction >> 8) & 0xF] & (~0xFFF)) != 0) {
                        chip->registers[0xF] = 1;
                    } else {
                        chip->registers[0xF] = 0;
                    }
                    chip->registers[(instruction >> 8) & 0xF] *= 2; // equivalent to x << 1
                break;
            }
        break;
        
        case 0x9: // skip if x != y
            if (chip->registers[(instruction >> 8) & 0xF] != chip->registers[(instruction >> 4) & 0xF]) {
                chip->pc += 2;
            }
        break;

        case 0xA: // set index
            chip->i = (instruction & 0xFFF);
        break;

        case 0xB: // another jump
            chip->pc = (instruction & 0xFFF) + chip->registers[0];
        break;
        
        case 0xC: // Vx = random number from 0 to 255 AND kk
            
        break;
        
        case 0xD: // display
            draw_sprite(chip, instruction);
            draw_display(chip->screen);
        break;

        case 0xE: // 
            
        break;
        
        case 0xF: // 
            
        break;
        
    }
}