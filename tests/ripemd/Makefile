all: test-ripemd.out

test-ripemd.out: test-ripemd.c rmd160.o misc.o
	gcc test-ripemd.c ripemd160.o misc.o -o test-ripemd.out

rmd160.o: ../../ripemd160.c ../../ripemd160.h
	gcc -c ../../ripemd160.c

misc.o: ../../misc.c ../../misc.h
	gcc -c ../../misc.c

clean:
	rm *.o *.out