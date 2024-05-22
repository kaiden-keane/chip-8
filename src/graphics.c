#include "graphics.h"
#include "debug.h"

void draw_sprite(struct Chip8 *chip, unsigned char x, unsigned char y, unsigned char n) {
    chip->registers[0xF] = 0;
    x = x % 64;
    y = y % 64;
    printf("x:%d y:%d n:%d\n", x, y, n);


    unsigned char val;
    // columns (n rows)
    for (int i = 0; i < n && y + i < 32; i++) {
        printf("i=%d\n", i);
        val = chip->mem[chip->i + i];
        printf("val: %d:", val);
        printBits(sizeof(val), &val);

        // rows
        // 8 because that is how many bits in char
        for (int j = 0; j < 8 && x + j < 64; j++) {
            // if bit is 1
            if ( ((val >> (7 - i)) & 0b1) == 1 ) {
                // if display (x, y) is 1
                if ( chip->display[y+i][x+j] == 1 ) {
                    chip->registers[0xF] = 1;
                    chip->display[y+i][x+j] = 0;
                } else {
                    chip->display[y+i][x+j] = 1;
                }
            }
        }
    }
}

void draw_display(struct Chip8 *chip) {
    clear_screen(&chip->screen);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            // printf("%d", chip->display[i][j]);
            if (chip->display[i][j] == 1) {
                draw_pixel(&chip->screen, j, i);
            }
        }
        // printf("\n");
    }
    // printf("\n");
    render_screen(&chip->screen);
}


void draw_pixel(RenderTexture2D *texture, int x, int y) {
    BeginTextureMode(*texture);
    DrawRectangle(SCREEN_SCALE * x, SCREEN_SCALE * y, SCREEN_SCALE, SCREEN_SCALE, WHITE);
    EndTextureMode();
}


void clear_screen(RenderTexture2D *texture) {
    BeginTextureMode(*texture);
    ClearBackground(BLACK);
    EndTextureMode();
}


void render_screen(RenderTexture2D *texture) {
    BeginDrawing();
    DrawTextureRec(texture->texture, (Rectangle) {0, 0, (float)texture->texture.width, (float)-texture->texture.height}, (Vector2) {SCREEN_SCALE, SCREEN_SCALE}, WHITE);
    EndDrawing();

}