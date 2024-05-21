#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include <stdlib.h>

#ifdef UNIX_BUILD
#include <unistd.h>
#endif

int main(int argc, char *argv[]) {
    struct Chip8 *chip8 = initialize_chip();
    read_rom(chip8, "roms/IBM Logo.ch8", 500);


    InitWindow(SCREEN_WIDTH * SCREEN_SCALE + 2*SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE + 2*SCREEN_SCALE, "CHIP-8");
    RenderTexture2D screen = LoadRenderTexture(SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE);

    // char screen_matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };
    while ( 1 ) {
        execute_instruction(chip8);

        }
        

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureRec(screen.texture, (Rectangle) { 0, 0, (float)screen.texture.width, (float)-screen.texture.height }, (Vector2) { 0, 0 }, WHITE);
        EndDrawing();
        usleep(100000);

    return 0;
}