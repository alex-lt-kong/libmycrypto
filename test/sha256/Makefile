sha256-test.out: sha256-test.cpp misc.o sha256.o
	g++ sha256-test.cpp misc.o sha256.o -o sha256-test.out

sha256.o: ../../sha256.c ../../sha256.h misc.o
	gcc -c ../../sha256.c

misc.o: ../../misc.c ../../misc.h
	gcc -c ../../misc.c

clean:
	rm *.o *.out