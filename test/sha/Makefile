test-sha.out: test-sha.cpp misc.o sha256.o sha1.o
	g++ test-sha.cpp misc.o sha256.o sha1.o -o test-sha.out

sha256.o: ../../sha256.c ../../sha256.h misc.o
	gcc -c ../../sha256.c

sha1.o: ../../sha1.c ../../sha1.h misc.o
	gcc -c ../../sha1.c

misc.o: ../../misc.c ../../misc.h
	gcc -c ../../misc.c

clean:
	rm *.o *.out