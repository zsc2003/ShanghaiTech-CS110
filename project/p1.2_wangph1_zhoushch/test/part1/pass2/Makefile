ifeq ($(VALGRIND), TRUE)
RISCV=../../../run-valgrind ../../../riscv
else
RISCV=../../../riscv
endif
IN_DIRS = ./in
OUT_DIRS = ./out
REF_DIRS = ./ref

INPUTS := $(shell find $(IN_DIRS) -name '*.symbol')
NAMES := $(foreach input, $(INPUTS), $(shell basename $(input) .symbol))

SUFFIX = basic text 

all: clean test grade

.PHONY: test grade clean 

test:
	@if test ! -d $(OUT_DIRS); then\
		mkdir $(OUT_DIRS);\
	fi
	@for n in $(NAMES); do \
		$(RISCV) assembler-p2 $(IN_DIRS)/$$n.symbol $(IN_DIRS)/$$n.original -b $(OUT_DIRS)/$$n.basic -t $(OUT_DIRS)/$$n.text -l $(OUT_DIRS)/$$n.log; \
		retval=$$?; \
		if [ "$(VALGRIND)" = "TRUE" ]; \
		then \
			if [ $$retval -eq 64 ]; \
			then \
				echo "[FAILED] Valgrind test $$n failed"; \
			else \
				echo "[PASSED] Valgrind test $$n passed"; \
			fi; \
		fi; \
	done

grade:
	@if [ "$(VALGRIND)" != "TRUE" ]; then \
	for n in $(NAMES); do \
		export current_file_erro=0; \
		if test -f $(REF_DIRS)/$$n.text; then \
			for s in $(SUFFIX); do \
				diff --brief $(OUT_DIRS)/$$n.$$s $(REF_DIRS)/$$n.$$s>/dev/null; \
				if [ $$? -eq 1 ]; then \
					echo "[FAILED] Test $$n failed when checking $$s file."; \
					export current_file_erro=1; \
				fi \
			done; \
		elif test -f $(REF_DIRS)/$$n.log; then \
			for s in $(ERROR_SUFFIX); do \
				diff --brief $(OUT_DIRS)/$$n.$$s $(REF_DIRS)/$$n.$$s>/dev/null; \
				if [ $$? -eq 1 ]; then \
					echo "[FAILED] Test $$n failed when checking $$s file."; \
					export current_file_erro=1; \
				fi \
			done; \
		else \
			echo "[INVALID] Test $$n is invalid. If this is a test case you designed, try to make a full list of files required for comparison. If this is a test case made by TA, which is extremely unlikely to happen, please post this to Piazza."; \
			export current_file_erro=1; \
		fi; \
		if [ $$current_file_erro -eq 0 ]; then \
			echo "[PASSED] Test $$n passed."; \
		fi \
	done; \
	fi;
clean:
	@-rm -f $(OUT_DIRS)/*

