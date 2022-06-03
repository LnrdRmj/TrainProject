main: main.o segmentiManager.o treno registro rbc
	gcc main.o segmentiManager.o -o main

main.o: main.c segmentiManager.h
	gcc -c main.c

registro: registro.c
	gcc registro.c -o registro

segmentiManager: segmentiManager.c
	gcc segmentiManager.c -o segmentiManager

segmentiManager.o: segmentiManager.c
	gcc -c segmentiManager.c

log: log.c
	gcc segmentiManager.c -o segmentiManager

log.o: log.c log.h
	gcc log.c -c
	
treno: treno.o segmentiManager.o log.o
	gcc treno.o segmentiManager.o log.o -o treno

treno.o: treno.c segmentiManager.h
	gcc treno.c -c

rbc: rbc.o socketHelper.o
	gcc rbc.o socketHelper.o -o rbc

rbc.o: rbc.c socketHelper.h
	gcc rbc.c -c

socketHelper: socketHelper.c
	gcc socketHelper.c -o socketHelper

socketHelper.o: socketHelper.c socketHelper.h
	gcc socketHelper.c -c

