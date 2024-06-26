#ifndef CHIP8_H
#define CHIP8_H


#define ENTRY_POINT 0x200
#define FONT_LOCATION 0x050


struct Chip8 {
    unsigned char *mem;         // program memory (ram?)
    unsigned char *V;           // registers
    unsigned short I;           // index register
    unsigned short pc;          // program counter
    unsigned char delay_timer;
    unsigned char sound_timer;

    struct Screen *screen;

    // stack
    unsigned short stack[16];   // instruction stack
    unsigned char sp;           // stack pointer
};


struct Chip8 *initialize_chip(); // initializes chip8
void deinitialize_chip(struct Chip8 *chip); // frees allocated memory

void load_fonts(struct Chip8 *chip, int address); //loads fonts into memory
void read_rom(struct Chip8 *chip8, char *filename, int mem_location); // reads rom into memory

int validate_key(int key); // validates keyboard key

unsigned short fetch_instruction(struct Chip8 *chip, unsigned short mem_location); // fetches next instruciton
void execute_instruction(struct Chip8 *chip); // executes instruction (from fetch_instruction)

#endif