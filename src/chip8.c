#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset

#include "raylib.h"
#include "graphics.h"

#include "chip8.h"

#include "debug.h"


int key_mappings[16] = {KEY_X, KEY_ONE, KEY_TWO, KEY_THREE,
                        KEY_Q, KEY_W, KEY_E, KEY_A,
                        KEY_S, KEY_D, KEY_Z, KEY_C,
                        KEY_FOUR, KEY_R, KEY_F, KEY_V};


/*
initializes all values of the chip
*/
struct Chip8 *initialize_chip() {
    struct Chip8 *chip = calloc(1, sizeof(struct Chip8));
    chip->mem = calloc(4096, sizeof(char));
    chip->V = malloc(sizeof(char) * 16);
    
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    
    chip->I = 0; // not needed just might aswell

    chip->sp = 0;
    chip->pc = ENTRY_POINT;
    memset(chip->stack, 0, sizeof(chip->stack));

    return chip;
}


/*
frees all allocated memory associated with the chip
*/
void deinitialize_chip(struct Chip8 *chip) {
    free(chip->V);
    free(chip->mem);
    free(chip->screen);
    free(chip);
}


/*
load the fonts into memory
*/
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


/*
read data from file into memory
*/
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

void update_timers(struct Chip8 *chip) {
    if (chip->delay_timer >= 1) {
        chip->delay_timer -= 1;
    }

    if (chip->sound_timer >= 1) {
        chip->sound_timer -= 1;
    }
}

/*
checks if a key is in our key mappings, if so it returns its equivalent
if not found returns -1
*/
int validate_key(int key) {
    for (int i = 0; i < 16; i++) {
        if (key == key_mappings[i]) {
            return i;
        }
    }
    return -1; // if not found
}


/*
execute instruction instruction at pc
*/
void execute_instruction(struct Chip8 *chip) {
    unsigned short instruction = (short)(((short)chip->mem[chip->pc]) << 8) | chip->mem[chip->pc + 1];
    print_instruction(instruction);
    chip->pc += 2;
    
    switch ((instruction >> (16-4)) & 0xF) {
        case 0x0: // clear screen
            // 00E0
            if ((instruction & 0xFF) == 0xE0) { // clear screen
                clear_screen(chip);
                render_screen(chip->screen);
            }
            // 00EE
            else if ((instruction & 0xFF) == 0xEE) {
                chip->pc = chip->stack[chip->sp];
                chip->sp -= 1;
            }
            break;

        // 1nnn
        case 0x1: // jump (set pc to nnn)
            chip->pc = instruction & 0xFFF;
            break;
        
        // 2nnn
        case 0x2: // call
            chip->sp += 1;
            chip->stack[chip->sp] = chip->pc;
            chip->pc = instruction & 0xFFF; 
            break;
        
        // 3xkk
        case 0x3: // skip if Vx = kk
            if (chip->V[(instruction >> 8) & 0xF] == (instruction & 0xFF)) {
                chip->pc += 2;
            }
            break;
        
        // 4xkk
        case 0x4: // skip if not equal
            if (chip->V[(instruction >> 8) & 0xF] != (instruction & 0xFF)) {
                chip->pc += 2;
            }
            break;
        
        // 5xy0
        case 0x5: // skip if Vx = Vy
            if (chip->V[(instruction >> 8) & 0xF] == chip->V[(instruction >> 4) & 0xF]) {
                chip->pc += 2;
            }
            break;
        
        // 6xkk
        case 0x6: // set registers
            chip->V[(instruction >> 8) & 0xF] = (instruction & 0xFF);
            break;

        // 7xkk
        case 0x7: // add registers
            chip->V[(instruction >> 8) & 0xF] += (instruction & 0xFF);
            break;

        case 0x8: 
            switch (instruction & 0xF) {
                // 8xy0
                case 0: // set Vx = Vy (8xy0)
                    chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 4) & 0xF];
                    break;

                // 8xy1
                case 1: // bitwise OR
                    chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 8) & 0xF] | chip->V[(instruction >> 4) & 0xF];
                    break;
                // 8xy2
                case 2: // bitwise AND
                    chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 8) & 0xF] & chip->V[(instruction >> 4) & 0xF];
                    break;
                
                // 8xy3
                case 3: // bitwise XOR
                    chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 8) & 0xF] ^ chip->V[(instruction >> 4) & 0xF];
                    break;

                // 8xy4
                case 4: // addition
                    ;
                    unsigned int result = chip->V[(instruction >> 8) & 0xF] + chip->V[(instruction >> 4) & 0xF];
                    chip->V[(instruction >> 8) & 0xF] = result & 0xFF;
                    chip->V[0xF] = 0;
                    if (result > 255) {
                        chip->V[0xF] = 1;
                    }
                    break;

                // 8xy5
                case 5: // subtraction
                    if (chip->V[(instruction >> 8) & 0xF] >= chip->V[(instruction >> 4) & 0xF]) {
                        chip->V[(instruction >> 8) & 0xF] -= chip->V[(instruction >> 4) & 0xF];
                        chip->V[0xF] = 1;
                    } else {
                        chip->V[(instruction >> 8) & 0xF] -= chip->V[(instruction >> 4) & 0xF];
                        chip->V[0xF] = 0;
                    }
                    break;

                // 8xy6
                case 6:
                    if ((chip->V[(instruction >> 8) & 0xF] & 1) == 1) {
                        chip->V[(instruction >> 8) & 0xF] /= 2; // equivalent to x >> 1
                        chip->V[0xF] = 1;
                    } else {
                        chip->V[(instruction >> 8) & 0xF] /= 2; // equivalent to x >> 1
                        chip->V[0xF] = 0;
                    }
                    break;

                // 8xy7
                case 7:
                    if (chip->V[(instruction >> 8) & 0xF] <= chip->V[(instruction >> 4) & 0xF]) {
                        chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 4) & 0xF] - chip->V[(instruction >> 8) & 0xF];
                        chip->V[0xF] = 1;
                    } else {
                        chip->V[(instruction >> 8) & 0xF] = chip->V[(instruction >> 4) & 0xF] - chip->V[(instruction >> 8) & 0xF];
                        chip->V[0xF] = 0;
                    }
                    break;

                // 8xyE
                case 0xE:
                   if ((chip->V[(instruction >> 8) & 0xF] & (0x80)) != 0) {
                        chip->V[(instruction >> 8) & 0xF] *= 2; // equivalent to x << 1
                        chip->V[0xF] = 1;
                    } else {
                        chip->V[(instruction >> 8) & 0xF] *= 2; // equivalent to x << 1
                        chip->V[0xF] = 0;
                    }
                    
                    break;
            }
            break;
        
        // 9XY0
        case 0x9: // skip if x != y
            if (chip->V[(instruction >> 8) & 0xF] != chip->V[(instruction >> 4) & 0xF]) {
                chip->pc += 2;
            }
            break;

        // Annn
        case 0xA: // set index = nnn
            chip->I = (instruction & 0xFFF);
            break;

        // Bnnn
        case 0xB: // jumps to nnn + v[0]
            chip->pc = (instruction & 0xFFF) + chip->V[0];
            break;
        
        // Cxkk
        case 0xC: // Vx = random number from 0 to 255 AND kk
            chip->V[(instruction >> 8) & 0xF] = (unsigned char)(rand() % 256) & (unsigned char)(instruction & 0xFF);
            break;
        
        // Dxyn
        case 0xD: // display
            draw_sprite(chip, instruction);
            draw_display(chip->screen);
            break;

        case 0xE:
            switch(instruction & 0xFF) {
                // Ex9E
                case 0x9E: // skip next instruction if v(x) = is pressed
                    if (IsKeyDown(key_mappings[chip->V[(instruction >> 8) & 0xF]])) {
                        chip->pc += 2;
                    }
                    break;
                
                // ExA1
                case 0xA1: // skip next instruction if v(x) = is NOT pressed
                    if (!IsKeyDown(key_mappings[chip->V[(instruction >> 8) & 0xF]])) {
                        chip->pc += 2;
                    }
                    break;
            }
            break;
        
        case 0xF:
            switch (instruction & 0xFF) {
                //Fx07
                case 0x07: // Set Vx = delay timer value.
                    chip->V[(instruction >> 8) & 0xF] = chip->delay_timer;
                    break;
                
                //Fx0A
                case 0x0A: // Wait for a key press, store the value of the key in Vx.
                           // All execution stops until a key is pressed, then the value of that key is stored in Vx.
                    ; // gets around expected expression
                    int key = GetKeyPressed();
                    int validated_key = validate_key(key); // ignore if not valid keystroke
                    if (key != 0 && validated_key != -1) {
                        chip->V[(instruction >> 8) & 0xF] = validated_key;
                    }
                    else {
                        chip->pc -= 2; // we still want to decriment timers so we will just go back if invalid
                    }
                    break;
                
                //Fx15
                case 0x15: // Set delay timer = Vx.
                     chip->delay_timer = chip->V[(instruction >> 8) & 0xF];
                    break;
                
                //Fx18
                case 0x18: // Set sound timer = Vx.
                    chip->sound_timer = chip->V[(instruction >> 8) & 0xF];
                    break;
                
                //Fx1E
                case 0x1E: // Set I = I + Vx.
                    chip->I += chip->V[(instruction >> 8) & 0xF];
                    break;
                
                //Fx29
                case 0x29: // Set I = location of sprite for digit Vx.
                    chip->I = FONT_LOCATION + ((instruction >> 8) & 0xF);
                    break;
                
                //Fx33
                case 0x33: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    ; // gets rid of expected expression error
                    unsigned char num = chip->V[(instruction >> 8) & 0xF];
                    chip->mem[chip->I] = num / 100;
                    chip->mem[chip->I+1] = (num / 10) % 10;
                    chip->mem[chip->I+2] = num % 10;
                    break;
                
                //Fx55
                case 0x55: // Store V V0 through Vx in memory starting at location I.
                    for (int i = 0; i < ((instruction >> 8) & 0xF)+1; i++) {
                        chip->mem[chip->I + i] = chip->V[i];
                    }
                    break;
                
                //Fx65
                case 0x65: // Read V V0 through Vx from memory starting at location I.
                    for (int i = 0; i < ((instruction >> 8) & 0xF)+1; i++) {
                        chip->V[i] = chip->mem[chip->I + i];
                    }
                    break;
            }
            break;     
    }
}