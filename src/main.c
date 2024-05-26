#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include "utils.h"

#include <stdlib.h>
#include "debug.h"

int main(int argc, char *argv[]) {
    // initialize chip
    struct Chip8 *chip8 = initialize_chip();
    read_rom(chip8, "roms/IBM Logo.ch8", ENTRY_POINT);
    load_fonts(chip8, FONT_LOCATION);
    chip8->pc = ENTRY_POINT;
    print_memory(chip8);

    // initialize window
    InitWindow(SCREEN_WIDTH * SCREEN_SCALE + 2*SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE + 2*SCREEN_SCALE, "CHIP-8");
    chip8->screen = LoadRenderTexture(SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE);
    clear_screen(&chip8->screen); // just to make sure it goes up properly
    
    render_screen(&chip8->screen);
    sleep(1);
    print_registers(chip8);

    while ( !WindowShouldClose() ) {
        execute_instruction(chip8);
        print_registers(chip8);
        printf("index: %d", chip8->i);
        printf("\n");
        

        sleep(0.5);
        draw_display(chip8);
    }
    printf("\n");
    return 0;
}