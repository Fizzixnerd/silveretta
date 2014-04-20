CC = gcc
INCLUDES = -Iinclude
#CPPFLAGS = -D_WDEBUG
CFLAGS = -std=c99 -Wall $(INCLUDES) $(CPPFLAGS)
LFLAGS = -ledit -lm
DEBUG = -ggdb

all : parsing

parsing : parsing.c obj/mpc.o obj/eval.o include/eval.h include/debug.h Makefile
	$(CC) $(CFLAGS) parsing.c obj/mpc.o obj/eval.o $(LFLAGS) -o parsing $(DEBUG)

obj/eval.o : eval.c obj/mpc.o include/eval.h include/mpc.h include/debug.h Makefile
	$(CC) $(CFLAGS) eval.c -c -o obj/eval.o $(DEBUG)

obj/traverse.o : traverse.c include/traverse.h Makefile
	$(CC) $(CFLAGS) traverse.c -c -o obj/traverse.o $(DEBUG)
