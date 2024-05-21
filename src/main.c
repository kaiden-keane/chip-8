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
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();

    while ( !WindowShouldClose() ) {
        execute_instruction(chip8);
        

        render_screen(&screen);
        usleep(100000);
        }

    return 0;
}