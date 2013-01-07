sha: sha.o sha_func.o
	gcc -o sha sha.o sha_func.o

sha.o: sha.c sha_func.o
	gcc -c sha.c

sha_func.o: sha_func.c sha_func.h
	gcc -c sha_func.c

clean:
	rm -rf *.o sha 
