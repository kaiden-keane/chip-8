#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "raylib.h"
#include "graphics.h"

#include "chip8.h"

#include "utils.h"

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

    srand(time(NULL)); // initialize for random
    // print_memory(chip);


    clock_t begin = clock();
    clock_t end;
    while ( !WindowShouldClose() ) {
        // check keyboard
        update_input(chip);

        execute_instruction(chip);
        
        render_screen(chip->screen);
        
        // mac = 1/512, windows = 1/256
        univ_sleep((float)1/256);
        
        // update timers at about 60hz
        end = clock();
        if (((double)(end - begin) / CLOCKS_PER_SEC) >= (double)1 / 60) {
            update_timers(chip);
            begin = clock();
        }
    }

    // free allocated memory
    deinitialize_chip(chip);
    
    return 0;
}