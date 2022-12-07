OPT = -O3 -Wall

all: test-ripemd.out

test-ripemd.out: test-ripemd.c rmd160.o misc.o
	gcc test-ripemd.c ripemd160.o misc.o -o test-ripemd.out $(OPT) -lcriterion

rmd160.o: ../../src/ripemd160.c ../../src/ripemd160.h
	gcc -c ../../src/ripemd160.c $(OPT)

misc.o: ../../src/misc.c ../../src/misc.h
	gcc -c ../../src/misc.c $(OPT)

clean:
	rm *.o *.out