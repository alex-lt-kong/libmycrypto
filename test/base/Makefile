all: test-base64.out test-base32.out

test-base64.out: test-base64.cpp base64.o
	g++ test-base64.cpp base64.o -o test-base64.out

test-base32.out: test-base32.cpp base32.o
	g++ test-base32.cpp base32.o -o test-base32.out

base64.o: ../../base64.c ../../base64.h
	g++ -c ../../base64.c

base32.o: ../../base32.c ../../base32.h
	g++ -c ../../base32.c

clean:
	rm *.o *.out