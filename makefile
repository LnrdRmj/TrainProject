main: main.o segmentiManager.o
	gcc main.o segmentiManager.o -o main

main.o: main.c segmentiManager.h
	gcc -c main.c

registro: registro.c
	gcc registro.c -o registro

segmentiManager: segmentiManager.c
	gcc segmentiManager.c -o segmentiManager

segmentiManager.o: segmentiManager.c
	gcc -c segmentiManager.c
