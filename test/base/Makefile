all: test-base32-64.out test-base58.out

test-base32-64.out: test-base32-64.c base64.o base32.o
	gcc test-base32-64.c base64.o base32.o -o test-base32-64.out -lm

test-base58.out: test-base58.cpp base58.o
	g++ test-base58.cpp base58.o -o test-base58.out

base64.o: ../../src/base64.c ../../src/base64.h
	gcc -c ../../src/base64.c

base58.o: ../../src/base58.c ../../src/base58.h
	gcc -c ../../src/base58.c

base32.o: ../../src/base32.c ../../src/base32.h
	gcc -c ../../src/base32.c

clean:
	rm *.o *.out