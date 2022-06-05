all: test-base32-64.out test-base58.out

test-base32-64.out: test-base32-64.cpp base64.o base32.o
	g++ test-base32-64.cpp base64.o base32.o -o test-base32-64.out

test-base58.out: test-base58.cpp base58.o
	g++ test-base58.cpp base58.o -o test-base58.out

base64.o: ../../base64.c ../../base64.h
	g++ -c ../../base64.c

base58.o: ../../base58.c ../../base58.h
	gcc -c ../../base58.c

base32.o: ../../base32.c ../../base32.h
	g++ -c ../../base32.c

clean:
	rm *.o *.out