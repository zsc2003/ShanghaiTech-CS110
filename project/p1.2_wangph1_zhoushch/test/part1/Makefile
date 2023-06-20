PASS1_DIRS = ./pass1
PASS2_DIRS = ./pass2

all: clean test

.PHONY: test grade clean 

test:
	@echo "----- Assembler test pass1 -----"
	@$(MAKE) -C pass1
	@echo "----- Assembler test pass2 -----"
	@$(MAKE) -C pass2


clean:
	@$(MAKE) -C pass1 clean
	@$(MAKE) -C pass2 clean