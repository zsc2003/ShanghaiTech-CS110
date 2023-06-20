CC = gcc
CFLAGS = -fopenmp -Wall -Wextra -std=gnu99
EXECUTABLES = hello matmul dotp

all: $(EXECUTABLES)

% : %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(EXECUTABLES)

.PHONY: all clean
