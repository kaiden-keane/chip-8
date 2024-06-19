#include "chip8.h"
#include "raylib.h"
#include "graphics.h"
#include "utils.h"

#include "stdio.h"

// CAN CURRENTLY CAN OVERFLOW REGISTERS

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
        printf("pc:%d\n", chip->pc);

        printf("stack: ");
        for (int i = 0; i < 16; i++) {
            printf("%d ", chip->stack[i]);
        }
        printf("  %d\n", chip->stack[chip->sp]);
        execute_instruction(chip);
        
        draw_display(screen);
        univ_sleep(0.0015);

        
        if (chip->delay_timer >= 1) {
            chip->delay_timer -= 1;
        }

        if (chip->sound_timer >= 1) {
            chip->sound_timer -= 1;
        }

        printf("\n");
    }

    deinitialize_chip(chip);
    
    return 0;
}