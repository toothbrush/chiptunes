CC = gcc
CFLAGS = -Wall -ggdb -std=c99
LIBS = -lncurses

MAIN = snd

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: clean

$(MAIN): $(OBJS)
	$(CC) $(LIBS) $(CFLAGS) -o $(MAIN) $^

# this is a suffix replacement rule for building .o's from .c's
# $<: the name of the first prerequisite of the rule (a .c file) and
# $@: the name of the target of the rule (an .o file)
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) *.o $(MAIN)
