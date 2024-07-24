CC = gcc
OBJDIR = build/obj/
OUTNAME = build/CHIP-8
CFLAGS = -Wall -Werror -Wextra -std=c11 -DUNIX_BUILD


prog: main.o graphics.o chip8.o debug.o
	$(CC) $(OBJDIR)/chip8.o $(OBJDIR)/debug.o $(OBJDIR)/graphics.o $(OBJDIR)/main.o -o $(OUTNAME) -L lib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a

debug.o: src/debug.c include/debug.h include/chip8.h
	$(CC) $(CFLAGS) -c src/debug.c -I include/ -o $(OBJDIR)$@

chip8.o: src/chip8.c include/chip8.h include/graphics.h include/debug.h
	$(CC) $(CFLAGS) -c src/chip8.c -I include/ -o $(OBJDIR)$@

graphics.o: src/graphics.c include/graphics.h include/chip8.h include/debug.h
	$(CC) $(CFLAGS) -c src/graphics.c -I include/ -o $(OBJDIR)$@

utils.o: src/utlils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -I include/ -o $(OBJDIR)$@

main.o: src/main.c include/graphics.h include/chip8.h include/utils.h include/debug.h
	$(CC) $(CFLAGS) -c src/main.c -I include/ -o $(OBJDIR)$@