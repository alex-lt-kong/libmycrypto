OPT = -O3 -Wall

test-sha.out: test-sha.c misc.o sha256.o sha1.o
	gcc test-sha.c misc.o sha256.o sha1.o -o test-sha.out -lcriterion $(OPT)

sha256.o: ../../src/sha256.c ../../src/sha256.h misc.o
	gcc -c ../../src/sha256.c $(OPT)

sha1.o: ../../src/sha1.c ../../src/sha1.h misc.o
	gcc -c ../../src/sha1.c $(OPT)

misc.o: ../../src/misc.c ../../src/misc.h
	gcc -c ../../src/misc.c $(OPT)

clean:
	rm *.o *.out