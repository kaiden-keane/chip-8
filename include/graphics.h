#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "chip8.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SCALE 16

void draw_sprite(struct Chip8 *chip, unsigned char x, unsigned char y, unsigned char n);
void draw_display(struct Chip8 *chip);

void draw_pixel(RenderTexture2D *texture, int x, int y);
void clear_screen(RenderTexture2D *texture);
void render_screen(RenderTexture2D *texture);


#endif