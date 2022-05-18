hmac-sha1.out: test-hmac.cpp utils.o hmac.o
	g++ test-hmac.cpp utils.o hmac.o sha256.o sha1.o -o hmac-test.out


hmac.o: ../../hmac.c ../../hmac.h utils.o sha256.o sha1.o
	gcc -c utils.o sha256.o sha1.o ../../hmac.c

sha256.o: ../../sha256.c ../../sha256.h utils.o
	g++ -c utils.o ../../sha256.c

sha1.o: ../../sha1.c ../../sha1.h utils.o
	g++ -c utils.o ../../sha1.c

utils.o: ../../utils.cpp ../../utils.h
	g++ -c ../../utils.cpp

clean:
	rm *.o *.out