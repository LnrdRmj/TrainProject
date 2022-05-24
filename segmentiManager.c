#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "segmentiManager.h"

#define PREFISSO "MA"

void creaFileSegmento(int);
void readSegmento(int, char*);
void getNumeroSegmentoDaStringa(int*, char*);
bool isLettera(char);
bool takeSegment(int);

void creaFileSegmento(int numSegmento){

  char *pathName;
  asprintf(&pathName, "segmenti/%s%d", PREFISSO, numSegmento);

  // int file = open(buf, O_CREAT|O_RDWR, 0666);

  FILE *file = fopen(pathName, "w");

  // Inizalizzazione a 0 dei file
  char str[] = "ciao";
  // fwrite(str, 1, sizeof(str), file);
  fputc('0', file);

  // printf("Scritto 0 in %s\n", pathName);

  fclose(file);
  // Setta i permessi corretti del file
  chmod(pathName, 0666);

}

void readSegmento(int segmento, char* occupato){

	char *fileName = malloc(3);

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	//printf("%s\n", fileName);
	FILE *fileSegmento = fopen(fileName, "r");

	if (fileSegmento == NULL) printf("Fail segmeno %i\n", segmento);

	*occupato = fgetc(fileSegmento);

}

void getNumeroSegmentoDaStringa(int *risultato, char *segmento) {

	// Salta le prime lettere
	while(isLettera(*segmento))
		segmento++;
	// Converte da stringa a numero
	*risultato = strtol(segmento, NULL, 10);

}

bool takeSegment(int segmento) {

	char *fileName = malloc(3);

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	//printf("%s\n", fileName);
	FILE *fileSegmento = fopen(fileName, "r+");
	if (fileSegmento == NULL) printf("Fail segmeno %i\n", segmento);

	if(fgetc(fileSegmento) == '0') {
		fputc('1', fileSegmento);
		return true;
	}

	return false;

}

bool isLettera(char daTestare) {

	return ( daTestare >= 'A' && daTestare <= 'Z' ) || 
			( daTestare >= 'a' && daTestare <= 'z' );

}