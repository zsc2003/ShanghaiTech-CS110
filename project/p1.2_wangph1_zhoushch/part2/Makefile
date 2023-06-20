CC = gcc
CFLAGS = -g -std=c89 -Wpedantic -Wall -Wextra -Werror
SRC = ../utils.c
OBJS = mem.o logs.o execute.o execute_utils.o emulator.o

ifeq ($(NEGLECT),TRUE)
CFLAGS := -g -std=c89 -w
else
CFLAGS := -g -std=c89 -Wpedantic -Wall -Wextra -Werror
endif

all: clean $(OBJS)

.PHONY: clean

$(OBJS): %.o: %.c

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@-rm -f *.o