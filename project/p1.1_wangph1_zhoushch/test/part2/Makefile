ifeq ($(VALGRIND), TRUE)
RISCV=../../run-valgrind ../../riscv
else
RISCV=../../riscv
endif
IN_DIRS = ./in
OUT_DIRS = ./out
REF_DIRS = ./ref

INPUTS := $(shell find $(IN_DIRS) -name '*.text')
NAMES := $(foreach input, $(INPUTS), $(shell basename $(input) .text))

SUFFIX = log 

all: clean test grade

.PHONY: test grade clean 

test:
	@if test ! -d $(OUT_DIRS); then\
		mkdir $(OUT_DIRS);\
	fi
	@echo "----- Emulator test -----"
	@for n in $(NAMES); do \
		$(RISCV) emulator $(IN_DIRS)/$$n.text $(IN_DIRS)/$$n.data -l $(OUT_DIRS)/$$n.log < $(IN_DIRS)/$$n.commands; \
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
		for s in $(SUFFIX); do \
			diff --brief $(OUT_DIRS)/$$n.$$s $(REF_DIRS)/$$n.$$s>/dev/null; \
			if [ $$? -eq 1 ]; then \
				echo "[FAILED] Test $$n failed when checking $$s file."; \
				export current_file_erro=1; \
			fi \
		done; \
		if [ $$current_file_erro -eq 0 ]; then \
			echo "[PASSED] Test $$n passed."; \
		fi \
	done; \
	fi;

clean:
	@-rm -f $(OUT_DIRS)/*
