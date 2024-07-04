#ifndef DEBUG_H
#define DEBUG_H

#include "chip8.h"
#include "stdio.h"

void print_memory(struct Chip8 *chip);
void printBits(size_t const size, void const * const ptr);
void print_instruction(unsigned short instruction);
void print_registers(struct Chip8 *chip);
void print_keys(struct Chip8 *chip);

#endif