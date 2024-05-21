#include "graphics.h"

void draw_pixel(RenderTexture2D *texture, int x, int y) {
    BeginTextureMode(*texture);
    DrawRectangle(x * SCREEN_SCALE, y * SCREEN_SCALE, SCREEN_SCALE, SCREEN_SCALE, WHITE);
    EndTextureMode();
}

void render_screen(RenderTexture2D *texture, char *screen_matrix) {

}