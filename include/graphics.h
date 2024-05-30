#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "chip8.h"

struct Chip8; // so the functions know it exists

struct Screen {
    int width;
    int height;
    int scale;

    RenderTexture2D texture;
    unsigned char display[32][64];
    
};

struct Screen *init_screen(struct Chip8 *chip);

void draw_sprite(struct Chip8 *chip, unsigned short instruction);
void draw_display(struct Screen *screen);

void draw_pixel(struct Screen *screen, int x, int y);
void clear_screen(RenderTexture2D *texture);
void render_screen(struct Screen *screen);


#endif