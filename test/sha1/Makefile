sha1-test.out: sha1-test.cpp sha1.o utils.o
	g++ sha1-test.cpp sha1.o utils.o -o sha1-test.out

sha1.o: ../../sha1.c ../../sha1.h
	g++ -c ../../sha1.c

utils.o: ../../utils.cpp ../../utils.h
	g++ -c ../../utils.cpp

clean:
	rm *.o *.out