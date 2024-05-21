#include "chip8.h"
#include "raylib.h"
#include "graphics.h"

#ifdef UNIX_BUILD
#include <unistd.h>
#endif


int main(int argc, char *argv[]) {
    chip8.pc = 500;
    read_rom("roms/IBM Logo.ch8", 500);


    InitWindow(SCREEN_WIDTH * SCREEN_SCALE + 2*SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE + 2*SCREEN_SCALE, "CHIP-8");
    RenderTexture2D screen = LoadRenderTexture(SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE);

    // char screen_matrix[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };
    unsigned short instruction;
    while ( 1 ) {
        instruction = fetch_instruction();
        
        switch (instruction >> 6) {
            case 0x0:
                break;

            case 0x1:
            break;

            case 0x6:
            break;

            case 0x7:
            break;

            case 0xA:
            break;

            case 0xD:
            break;

        }
        

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureRec(screen.texture, (Rectangle) { 0, 0, (float)screen.texture.width, (float)-screen.texture.height }, (Vector2) { 0, 0 }, WHITE);
        EndDrawing();
        usleep(100000);
    }

    return 0;
}