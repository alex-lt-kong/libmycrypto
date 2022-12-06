test-sha.out: test-sha.c misc.o sha256.o sha1.o
	gcc test-sha.c misc.o sha256.o sha1.o -o test-sha.out -lcunit

sha256.o: ../../src/sha256.c ../../src/sha256.h misc.o
	gcc -c ../../src/sha256.c

sha1.o: ../../src/sha1.c ../../src/sha1.h misc.o
	gcc -c ../../src/sha1.c

misc.o: ../../src/misc.c ../../src/misc.h
	gcc -c ../../src/misc.c

clean:
	rm *.o *.out