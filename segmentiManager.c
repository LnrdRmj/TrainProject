#include <stdio.h>
#include <stdbool.h>
#include "segmentiManager.h"

#define PREFISSO "MA"

void creaFileSegmento(int);
void readSegmento(int, char*);
void getNumeroSegmentoDaStringa(int*, char*);
bool isLettera(char);

void creaFileSegmento(int numSegmento){

  char *pathName;
  asprintf(&pathName, "segmenti/%s%d", PREFISSO, numSegmento);
  // int file = open(buf, O_CREAT|O_RDWR, 0666);

  FILE *file = fopen(pathName, "w+");

  // Inizalizzazione a 0 dei file
  fwrite("0", 1, 1, file);

  fclose(file);
  // Setta i permessi corretti del file
  chmod(pathName, 0666);

}

void readSegmento(int segmento, char* occupato){

	char *fileName = malloc(3);

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	printf("%s\n", fileName);

}

void getNumeroSegmentoDaStringa(int *risultato, char *segmento) {

	// Salta le prime lettere
	while(isLettera(*segmento))
		segmento++;

	// Converte da stringa a numero
	*risultato = strtol(segmento, NULL, 10);

}

bool isLettera(char daTestare) {

	return ( daTestare >= 'A' && daTestare <= 'Z' ) || 
			( daTestare >= 'a' && daTestare <= 'z' );

}