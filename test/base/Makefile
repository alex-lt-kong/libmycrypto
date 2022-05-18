test-base64.out: test-base64.cpp base64.o
	g++ test-base64.cpp base64.o -o test-base64.out

base64.o: ../../base64.c ../../base64.h
	g++ -c ../../base64.c

clean:
	rm *.o *.out