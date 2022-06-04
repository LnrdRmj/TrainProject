main: main.o segmentiManager.o treno registro rbc
	gcc main.o segmentiManager.o -o main

main.o: main.c segmentiManager.h
	gcc main.c -c

registro: registro.o socketHelper.o
	gcc registro.c socketHelper.o -o registro

registro.o: registro.c socketHelper.h
	gcc registro.c -c

segmentiManager: segmentiManager.o
	gcc segmentiManager.o -o segmentiManager

segmentiManager.o: segmentiManager.c
	gcc segmentiManager.c -c

log: log.c
	gcc segmentiManager.c -o segmentiManager

log.o: log.c log.h
	gcc log.c -c
	
treno: treno.o segmentiManager.o log.o socketHelper.o
	gcc treno.o segmentiManager.o log.o socketHelper.o -o treno

treno.o: treno.c segmentiManager.h log.h socketHelper.h
	gcc treno.c -c

rbc: rbc.o socketHelper.o segmentiManager.o
	gcc rbc.o socketHelper.o segmentiManager.o -o rbc

rbc.o: rbc.c socketHelper.h segmentiManager.h
	gcc rbc.c -c

socketHelper: socketHelper.c
	gcc socketHelper.c -o socketHelper

socketHelper.o: socketHelper.c socketHelper.h
	gcc socketHelper.c -c

