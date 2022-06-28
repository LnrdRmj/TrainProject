#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "segmentiManager.h"

#define PREFISSO "MA"

void creaFileSegmento(int);
void readSegmento(int, char *);
int getNumeroSegmentoDaStringa(char *);
bool isLettera(char);
bool segmentoIsLibero(int);
bool takeSegmento(int);
void freeSegmento(int);

// Crea e inizializza il file per memorizzare lo stato di un segmento 
void creaFileSegmento(int numSegmento){

    char *pathName;
    asprintf(&pathName, "segmenti/%s%d", PREFISSO, numSegmento);

    FILE *file = fopen(pathName, "w");

    // Inizalizzazione a 0 dei file
    fputc('0', file);
    fclose(file);

    // Setta i permessi corretti al file
    chmod(pathName, 0666);
}

// Legge il contenuto di un file segmento
void readSegmento(int segmento, char* occupato){

	char* fileName = malloc(3);

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	//printf("%s\n", fileName);
	FILE* fileSegmento = fopen(fileName, "r");

    if (fileSegmento == NULL)
        printf("Fail segmeno %i\n", segmento);

    *occupato = fgetc(fileSegmento);
}

// Trasforma il nomde di un segmento in un numero
// Esempio: MA3 => 3
int getNumeroSegmentoDaStringa(char* segmento) {

	// Salta le prime lettere
	while(isLettera(*segmento))
		segmento++;
	// Converte da stringa a numero
	return strtol(segmento, NULL, 10);
}

// Verifica che un segmento sia libero o meno
// Ritorna vero se il segmento e' liberom, false altrimenti
bool segmentoIsLibero(int segmento) {

	char* fileName;

	asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);
	FILE* fileSegmento = fopen(fileName, "r");

    bool result = (fgetc(fileSegmento) == '0');
    fclose(fileSegmento);

    return result;
}

// Prende il segmento, cioe' imposta a 1 il contenuto di un file segmento
bool takeSegmento(int segmento) {

	char* fileName = malloc(20);

	sprintf(fileName, "segmenti/%s%d", PREFISSO, segmento);

	FILE *fileSegmento = fopen(fileName, "r+");
	if (fileSegmento == NULL) printf("Fail segmento %i\n", segmento);

	fputc('1', fileSegmento);

	fclose(fileSegmento);
	return false;
}

// Mette a 0 il segmento liberandolo
void freeSegmento(int segmento) {

    char *fileName;
    asprintf(&fileName, "segmenti/%s%d", PREFISSO, segmento);

    FILE *fileSegmento = fopen(fileName, "w");
    fputc('0', fileSegmento);

    fclose(fileSegmento);
}

// Un metodo helper per verificare se un certo carattere e' una lettera o meno
bool isLettera(char daTestare) {

	return ( daTestare >= 'A' && daTestare <= 'Z' ) || 
			( daTestare >= 'a' && daTestare <= 'z' );
}