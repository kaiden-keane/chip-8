#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    printf("%s", argv[1]);
    if (argc != 2) {
        printf("plz supply a filename lol\n");
        return 1;
    }
    
    // initialize chip
    struct Chip8 *chip = initialize_chip();
    read_rom(chip, argv[1], ENTRY_POINT);
    load_fonts(chip, FONT_LOCATION);

    // initialize window
    struct Screen *screen = init_screen(chip);

    while ( !WindowShouldClose() ) {
        execute_instruction(chip);
        
        draw_display(screen);
    }

    deinitialize_chip(chip);
    
    return 0;
}