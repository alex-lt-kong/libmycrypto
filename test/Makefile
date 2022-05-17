sha256-test.out: sha256-test.cpp utils.o sha256.o
	g++ sha256-test.cpp utils.o sha256.o -o sha256-test.out

sha256.o: ../sha256.c utils.o
	gcc -c ../sha256.c

utils.o: ../utils.cpp ../utils.h
	g++ -c ../utils.cpp

