hmac-sha1.out: test-hmac.cpp misc.o hmac.o
	g++ test-hmac.cpp misc.o hmac.o sha256.o sha1.o -o hmac-test.out


hmac.o: ../../hmac.c ../../hmac.h misc.o sha256.o sha1.o
	gcc -c misc.o sha256.o sha1.o ../../hmac.c

sha256.o: ../../sha256.c ../../sha256.h misc.o
	g++ -c misc.o ../../sha256.c

sha1.o: ../../sha1.c ../../sha1.h misc.o
	g++ -c misc.o ../../sha1.c

misc.o: ../../misc.cpp ../../misc.h
	g++ -c ../../misc.cpp

clean:
	rm *.o *.out