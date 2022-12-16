OPT = -O3 -Wall

hmac-sha1.out: test-hmac.c misc.o hmac.o sha256.o sha1.o
	gcc test-hmac.c misc.o hmac.o sha256.o sha1.o -o hmac-test.out $(OPT) -lcriterion

hmac.o: ../../src/hmac.c ../../src/hmac.h
	gcc -c ../../src/hmac.c $(OPT)

sha256.o: ../../src/sha256.c ../../src/sha256.h
	gcc -c ../../src/sha256.c $(OPT)

sha1.o: ../../src/sha1.c ../../src/sha1.h
	gcc -c ../../src/sha1.c $(OPT)

misc.o: ../../src/misc.c ../../src/misc.h
	gcc -c ../../src/misc.c $(OPT)

clean:
	rm *.o *.out