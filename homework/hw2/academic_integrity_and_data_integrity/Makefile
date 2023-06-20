# see <https://makefiletutorial.com/> for a comprehensive & comprehendible tutorial on GNU make

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic

test_src = tester/file_tester.c tester/dir_tester.c tester/explorer_tester.c

src_files = file.c dir.c node.c explorer.c
obj_files = file.o dir.o node.o explorer.o
static_obj_files  = file.o-static  dir.o-static  node.o-static  explorer.o-static
dynamic_obj_files = file.o-dynamic dir.o-dynamic node.o-dynamic explorer.o-dynamic
static_lib  = libexplorer.a
dynamic_lib = libexplorer.so


all: $(obj_files) $(static_obj_files) $(dynamic_obj_files)
.PHONY: all 

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o-static : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o-dynamic : %.c
	$(CC) $(CFLAGS) -fpic -shared -c $< -o $@

clean:
	rm -f $(obj_files)
	rm -f $(static_obj_files) $(static_lib)
	rm -f $(dynamic_obj_files) $(dynamic_lib)
	rm -f test.out test.static.out test.dynamic.out

# the statically linked library (or archive library)
$(static_lib): $(static_obj_files)
	ar rcs $@ $^

# the dynamically linked library (or shared object)
$(dynamic_lib): $(dynamic_obj_files)
	$(CC) -fpic -shared $^ -o $@

test.out:         main.c $(test_src) $(obj_files)
	$(CC) $(CFLAGS) $^ -o $@

test.static.out:  main.c $(test_src) $(dynamic_obj_files) $(static_lib)
	$(CC) $(CFLAGS) $^ -o $@ -static 
# $(CC) $(CFLAGS) -static main.c $(test_src) $(dynamic_obj_files) $(static_lib) -o test.static.out

test.dynamic.out: main.c $(test_src) $(dynamic_lib)
	$(CC) $(CFLAGS) $^ -o $@ -Wl,-rpath=.
#	$(CC) $(CFLAGS) main.c $(test_src) $(static_obj_files) $(dynamic_lib) -o test.dynamic.out

test-memcheck:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./test.out

test.static-memcheck:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./test.static.out

test.dynamic-memcheck:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./test.dynamic.out