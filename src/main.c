#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include "utils.h"

#include <stdlib.h>
#include "debug.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("plz supply a filename lol\n");
        return 1;
    }
    
    // initialize chip
    struct Chip8 *chip8 = initialize_chip();
    read_rom(chip8, argv[1], ENTRY_POINT);
    load_fonts(chip8, FONT_LOCATION);
    chip8->pc = ENTRY_POINT;

    // initialize window
    InitWindow(SCREEN_WIDTH * SCREEN_SCALE + 2*SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE + 2*SCREEN_SCALE, "CHIP-8");
    chip8->screen = LoadRenderTexture(SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE);

    while ( !WindowShouldClose() ) {
        execute_instruction(chip8);
        
        draw_display(chip8);
    }

    deinitialize_chip(chip8);
    
    return 0;
}