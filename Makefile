# *************************** #
#   Turtle Run 
#     - 10/24/2025
#     - Bryce Keen
# *************************** #

CC=gcc
CFLAGS=-O -Wall

all: turtle-run

turtle-run: turtle_run.c 
	$(CC) $(CFLAGS) -o turtle-run turtle_run.c -lncurses

clean:
	rm -f turtle-run

distclean: clean
