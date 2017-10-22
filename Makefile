#!bin/bash
main:main.o bd_sov.o
	gcc -o main main.o bd_sov.o
main.o:main.c
	gcc -c main.c
bd_sov.o:bd_sov.c bd_sov.h
	gcc -c bd_sov.c

.PHONY:clean
clean:
	rm *.o main

