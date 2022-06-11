all: test-ripemd.out

test-ripemd.out: test-ripemd.c rmd160.o
	gcc test-ripemd.c rmd160.o -o test-ripemd.out

rmd160.o: ../../rmd160.c ../../rmd160.h
	gcc -c ../../rmd160.c

clean:
	rm *.o *.out