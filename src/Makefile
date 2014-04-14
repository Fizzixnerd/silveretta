CC = gcc
INCLUDES = -Iinclude
CFLAGS = -std=c99 -Wall $(INCLUDES)
LFLAGS = -ledit -lm
DEBUG = -ggdb

all : parsing

parsing : parsing.c mpc.c obj/eval.o include/eval.h
	$(CC) $(CFLAGS) parsing.c obj/mpc.o obj/eval.o $(LFLAGS) -o parsing $(DEBUG)

obj/eval.o : eval.c include/eval.h include/mpc.h
	$(CC) $(CFLAGS) eval.c -c -o obj/eval.o $(DEBUG)

obj/traverse.o : traverse.c traverse.h
	$(CC) $(CFLAGS) traverse.c -c -o obj/traverse.o $(DEBUG)
