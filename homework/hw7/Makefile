all: simulator

hw7.tar: cashier.c
	tar cf $@ $<

simulator: cashier.c main.c
	gcc -Wall -Wextra -std=c11 -g cashier.c main.c -o $@

clean:
	rm -rf simulator

.PHONY: clean
