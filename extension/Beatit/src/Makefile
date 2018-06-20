# CC      = gcc
# CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
# LIBS = -lX11
# BUILD	  = render
#
#
# all: $(BUILD)
#
# render:
# 	$(CC) gameloop.c render.c gfx.c readbmp.c testLoop.c list.c -o game $(LIBS)

game: menu.c level.c gameloop.c player.c enemy.c gamelogic.c gfx.c list.c renderio.c render.c renderinit.c renderdef.c
	gcc -g menu.c level.c gameloop.c player.c enemy.c gamelogic.c gfx.c list.c renderio.c render.c renderinit.c renderdef.c -o game -lX11

force:
	gcc -g menu.c level.c gameloop.c player.c enemy.c gamelogic.c gfx.c list.c renderio.c render.c renderinit.c renderdef.c -o game -lX11
