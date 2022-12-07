hmac-sha1.out: test-hmac.c misc.o hmac.o sha256.o sha1.o
	gcc test-hmac.c misc.o hmac.o sha256.o sha1.o -o hmac-test.out

hmac.o: ../../hmac.c ../../hmac.h
	gcc -c ../../hmac.c

sha256.o: ../../sha256.c ../../sha256.h
	gcc -c ../../sha256.c

sha1.o: ../../sha1.c ../../sha1.h
	gcc -c ../../sha1.c

misc.o: ../../misc.c ../../misc.h
	gcc -c ../../misc.c

clean:
	rm *.o *.out