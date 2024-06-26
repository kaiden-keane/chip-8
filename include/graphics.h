#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

struct Chip8; // so the functions know it exists

struct Screen {
    int width;      // screen width
    int height;     // screen height
    int scale;      // real pixels to simulated pixel

    RenderTexture2D texture;        // screen texture
    unsigned char display[32][64];  // simulated display
    
};

void init_screen(struct Chip8 *chip); // initializes values of the screen

void draw_sprite(struct Chip8 *chip, unsigned short instruction); // draws sprite based on instruction
void draw_display(struct Screen *screen); // draws display to screen texture

void clear_screen(RenderTexture2D *texture); // makes screen texture all black
void render_screen(struct Screen *screen); // renders screen texture to screen


#endif