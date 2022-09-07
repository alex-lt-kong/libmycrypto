all: base64-decode.out

base64-decode.out: base64-decode.c base64.o
	gcc base64-decode.c base64.o -o base64-decode.out -lm -O3

base64.o: ../src/base64.c ../src/base64.h
	gcc -c ../src/base64.c

clean:
	rm *.o *.out