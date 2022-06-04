sha1-test.out: sha1-test.cpp sha1.o misc.o
	g++ sha1-test.cpp sha1.o misc.o -o sha1-test.out

sha1.o: ../../sha1.c ../../sha1.h
	g++ -c ../../sha1.c

misc.o: ../../misc.cpp ../../misc.h
	g++ -c ../../misc.cpp

clean:
	rm *.o *.out