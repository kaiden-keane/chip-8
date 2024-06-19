#include "graphics.h"
#include <stdlib.h>
#include <string.h> // for memset

#include "debug.h"



struct Screen *init_screen(struct Chip8 *chip) {
    struct Screen *screen = malloc (sizeof(struct Screen));

    screen->height = 32;
    screen->width = 64;
    screen->scale = 16;
    
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen->scale * (screen->width + 2), screen->scale * (screen->height + 2), "CHIP-8");
    screen->texture = LoadRenderTexture(screen->width * screen->scale, screen->height * screen->scale);

    memset(screen->display, 0, sizeof(screen->display));

    chip->screen = screen;

    return screen;
}

void draw_sprite(struct Chip8 *chip, unsigned short instruction) {
    unsigned char x = chip->V[(instruction >> 8) & 0xF] % 64;
    unsigned char y = chip->V[(instruction >> 4) & 0xF] % 32;
    unsigned char n = instruction & 0xF;
    chip->V[0xF] = 0;

    unsigned char val;
    // columns (n rows)
    for (int i = 0; i < n && y + i < 32; i++) {
        val = chip->mem[chip->I + i];

        // rows (8 bits in char)
        for (int j = 0; j < 8 && x + j < 64; j++) {
            // if bit is 1
            if ( ((val >> (7 - j)) & 0b1) == 1 ) {
                // if display (x, y) is 1
                if ( chip->screen->display[y+i][x+j] == 1 ) {
                    chip->V[0xF] = 1;
                    chip->screen->display[y+i][x+j] = 0;
                } else {
                    chip->screen->display[y+i][x+j] = 1;
                }
            }
        }
    }
}

void draw_display(struct Screen *screen) {
    clear_screen(&screen->texture);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (screen->display[i][j] == 1) {
                draw_pixel(screen, j, i);
            }
        }
    }
    render_screen(screen);
}


void draw_pixel(struct Screen *screen, int x, int y) {
    BeginTextureMode(screen->texture);
    DrawRectangle(screen->scale * x, screen->scale * y, screen->scale, screen->scale, WHITE);
    EndTextureMode();
}


void clear_screen(RenderTexture2D *texture) {
    BeginTextureMode(*texture);
    ClearBackground(BLACK);
    EndTextureMode();
}


void render_screen(struct Screen *screen) {
    BeginDrawing();
    DrawTextureRec(screen->texture.texture, (Rectangle) {0, 0, (float)screen->texture.texture.width, (float)-screen->texture.texture.height}, (Vector2) {screen->scale, screen->scale}, WHITE);
    EndDrawing();
}