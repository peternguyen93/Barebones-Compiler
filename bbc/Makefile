CFLAGS = -g -Wall -Wextra
CC=gcc
YACC = bison
YFLAGS = -d -v -t

LEX = flex
LFLAGS =
# Use -d flag for debug:
# LFLAGS = -d

LDFLAGS = -g
LOADLIBES = -lm

%.tab.c %.tab.h %.output: %.y
	$(YACC) $(YFLAGS) $<

bbc: barebones_parser.o parser.tab.o scanner.o barebones_utils.o
	$(CC) $(LDFLAGS) barebones_parser.o parser.tab.o scanner.o barebones_utils.o $(LOADLIBES) -o bbc

barebones_parser.o: barebones_parser.c barebones.h parser.tab.h util.h

barebones_utils.o: barebones_utils.c util.h

scanner.o: scanner.l barebones.h util.h parser.tab.h

parser.tab.o: parser.y barebones.h

clean:
	rm *.o
	rm bbc