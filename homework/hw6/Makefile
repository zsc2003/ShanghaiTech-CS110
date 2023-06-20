# see <https://makefiletutorial.com/> for a comprehensive & comprehensible tutorial on GNU make

CC = gcc
CFLAGS = -lpthread -Wall -Wextra -Werror -Wpedantic -std=gnu11

test_src = test/trivial.c
out = test.out

src_files = ringbuffer.c threadpool.c

all: memcheck
.PHONY: all clean build run memcheck

build: clean
	$(CC) $(test_src) $(src_files) $(CFLAGS) -o $(out)

run: build
	./$(out)

memcheck: build
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(out)

clean:
	rm -f test.out

submit:
	tar -cvf hw6_threadpool.tar $(src_files)