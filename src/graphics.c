#include "graphics.h"

void draw_pixel(RenderTexture2D *texture, int x, int y) {
    BeginTextureMode(*texture);
    DrawRectangle(SCREEN_SCALE * x, SCREEN_SCALE * y, SCREEN_SCALE, SCREEN_SCALE, WHITE);
    EndTextureMode();
}

void render_screen(RenderTexture2D *texture) {
    BeginDrawing();
    DrawTextureRec(texture->texture, (Rectangle) {0, 0, (float)texture->texture.width, (float)-texture->texture.height}, (Vector2) {SCREEN_SCALE, SCREEN_SCALE}, WHITE);
    EndDrawing();

}