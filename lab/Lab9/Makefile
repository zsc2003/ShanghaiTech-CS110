all: clean part1 part2_3 part2_3_opt

part1:
	gcc -g -Wall -Wextra -Wpedantic -Werror -std=gnu11 -O0 -o part1 part1.c -march=native

part2_3:
	gcc -g -Wall -Wextra -Wpedantic -Werror -std=gnu11 -O0 -o part2_3 part2_3.c -march=native

part2_3_opt:
	gcc -g -Wall -Wextra -Wpedantic -Werror -std=gnu11 -O3 -o part2_3_opt part2_3.c -march=native

test: part2_3 part2_3_opt
	@echo "Running part2_3.c with -O0"
	@./part2_3
	@echo "Running part2_3.c with -O3"
	@./part2_3_opt

clean:
	rm -f part1 part2_3 part2_3_opt

.PHONY: part1 part2_3 part2_3_opt clean all