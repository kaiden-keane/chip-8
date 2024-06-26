#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include "utils.h"

#include "stdio.h"

#include "debug.h"

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
    init_screen(chip);


    while ( !WindowShouldClose() ) {

        execute_instruction(chip);

        // decrement timers
        if (chip->delay_timer >= 1) {
            chip->delay_timer -= 1;
        }

        if (chip->sound_timer >= 1) {
            chip->sound_timer -= 1;
        }

        render_screen(chip->screen);
    }

    deinitialize_chip(chip);
    
    return 0;
}