#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "segmentiManager.h"

#define PREFISSO "MA"

void creaFileSegmento(int);
void readSegmento(int, char*);
int getNumeroSegmentoDaStringa(char*);
bool isLettera(char);
bool segmentoIsLibero(int);
bool takeSegmento(int);
void freeSegmento(int);

void creaFileSegmento(int numSegmento){

  char *pathName;
  asprintf(&pathName, "segmenti/%s%d", PREFISSO, numSegmento);

  FILE *file = fopen(pathName, "w");

  // Inizalizzazione a 0 dei file
  fputc('0', file);
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

int getNumeroSegmentoDaStringa(char *segmento) {

	// Salta le prime lettere
	while(isLettera(*segmento))
		segmento++;
	// Converte da stringa a numero
	return strtol(segmento, NULL, 10);

}

bool segmentoIsLibero(int segmento) {

	char *fileName;

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	FILE *fileSegmento = fopen(fileName, "r");

	bool result = (fgetc(fileSegmento) == '0');
	fclose(fileSegmento);

	return result;

}

bool takeSegmento(int segmento) {

	char *fileName;

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	//printf("%s\n", fileName);
	FILE *fileSegmento = fopen(fileName, "r+");
	if (fileSegmento == NULL) printf("Fail segmento %i\n", segmento);

	// if(fgetc(fileSegmento) == '0') {
	// 	// Mi riposiziono all'inizio del file
	// 	fseek(fileSegmento, 0, SEEK_SET);
	fputc('1', fileSegmento);

	// 	fclose(fileSegmento);

	// 	return true;
	// }

	fclose(fileSegmento);
	return false;

}

// Mette a 0 il segmento e lo "libera"
void freeSegmento(int segmento) {

	char *fileName;
	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	
	FILE *fileSegmento = fopen(fileName, "w");
	fputc('0', fileSegmento);
	
	fclose(fileSegmento);

}

bool isLettera(char daTestare) {

	return ( daTestare >= 'A' && daTestare <= 'Z' ) || 
			( daTestare >= 'a' && daTestare <= 'z' );

}