#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SCALE 16

int draw_pixel(RenderTexture2D *texture, int x, int y);
int render_screen(RenderTexture2D *texture, char *screen_matrix);


#endif