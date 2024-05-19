#include "chip8.h"
#include "raylib.h"

int main(int argc, char *argv[]) {
    const int screen_width = 64;
    const int screen_height = 32;
    const int screen_scale = 15;
    InitWindow(screen_width * screen_scale, screen_height * screen_scale, "CHIP-8");

    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }

    return 0;
}

int read_rom(char *file_name) {
    return 0;
}