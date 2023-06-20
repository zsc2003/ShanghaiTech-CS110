CC = gcc
CFLAGS = -O0 
all : base_test fast_test

clean:
	@-rm -f *.o log_base log_fast
	
base:
	@-rm -f *.o log_base
	$(CC) $(CFLAGS) -o log_base log_base.c 

fast:
	@-rm -f *.o log_fast
	$(CC) $(CFLAGS) -o log_fast log_fast.c $(LIBS)

check:
	@-rm -f *.o test_accuracy
	$(CC) $(CFLAGS) -o test_accuracy test_accuracy.c $(LIBS)

base_test: base
	./log_base 

fast_test: fast
	./log_fast

check_test: check
	./test_accuracy test_base.jpg test_fast.jpg

 