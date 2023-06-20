CC = gcc
CFLAGS = -g -std=c89 -Wpedantic -Wall -Wextra -Werror
SRC = utils.c
PART1_OBJS = part1/translate.o part1/translate_utils.o part1/tables.o part1/assembler.o
PART2_OBJS = part2/mem.o part2/logs.o part2/execute.o part2/execute_utils.o part2/emulator.o
ifndef VERBOSE
SILENT = --no-print-directory
endif

all: part1

.PHONY: riscv part1 part2 test clean test_valgrind

riscv:
	@$(MAKE) -C part1
	@$(MAKE) -C part2
	@$(CC) $(CFLAGS) -o $@ riscv.c $(SRC) $(PART1_OBJS) $(PART2_OBJS)

part1:
	@$(MAKE) -C part1 $(SILENT)
	@$(MAKE) -C part2 NEGLECT=TRUE $(SILENT)
	@$(CC) $(CFLAGS) -o riscv riscv.c $(SRC) $(PART1_OBJS) $(PART2_OBJS)

part2:
	@$(MAKE) -C part1 NEGLECT=TRUE $(SILENT)
	@$(MAKE) -C part2 $(SILENT)
	@$(CC) $(CFLAGS) -o riscv riscv.c $(SRC) $(PART1_OBJS) $(PART2_OBJS)

empty:
	@$(MAKE) -C part1 NEGLECT=TRUE $(SILENT)
	@$(MAKE) -C part2 NEGLECT=TRUE $(SILENT)
	@$(CC) $(CFLAGS) -o riscv riscv.c $(SRC) $(PART1_OBJS) $(PART2_OBJS)

test_part1: part1
	@$(MAKE) -C test/part1 $(SILENT)

test_part1_valgrind: part1
	@$(MAKE) -C test/part1 $(SILENT) VALGRIND=TRUE

test_part2: part2
	@$(MAKE) -C test/part2 $(SILENT)

test_part2_valgrind: part2
	@$(MAKE) -C test/part2 $(SILENT) VALGRIND=TRUE

test: test_part1

test_valgrind: test_part1_valgrind

clean:
	@$(MAKE) -C part1 clean
	@$(MAKE) -C part2 clean
	@$(MAKE) -C test/part1 clean
	@$(MAKE) -C test/part2 clean

	@-rm -f *.o riscv
