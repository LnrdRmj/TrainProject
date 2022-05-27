#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdbool.h>

#include "segmentiManager.h"

#define SERVER_NAME "serveRegistro"
#define PREFISSO_FILE_SEGMENTO "MA"

void getCammino(char *, char *);
void startJourney(char *, long);
bool isStazione(char *);
void liberaSegmento(char *);
int splitString(char *, const char *, char *[]);

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[1];
  char *mappa = argv[2];

  getCammino(numTreno, mappa);

  return 0;

}

void getCammino(char *numTreno, char *mappa){

  struct sockaddr_un registro;
  registro.sun_family = AF_UNIX;
  strcpy(registro.sun_path, SERVER_NAME);

  int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

  int result;
  do{
    int serverLen = sizeof(registro);
    result = connect(serverFd, (struct sockaddr*) &registro, serverLen);
    if(result == -1) {
    	sleep(1);
    }
  }while(result == -1);

  char *tmp;
  long lnumeroTreno = strtol(numTreno, &tmp, 10);

  char *dataForServer = strcat(numTreno, mappa);
  write(serverFd, dataForServer, strlen(dataForServer) + 1);

  char *cammino = malloc(100);
  read(serverFd, cammino, 100);
  // printf("Il cammino del treno %s e': %s\n", numTreno, cammino);

  startJourney(cammino, lnumeroTreno);

  close(serverFd);

}

void startJourney(char * cammino, long numeroTreno){

	// Questo array conterra' il percorso sottoforma di array
	char *passiCammino[10];

	int numPassi = splitString(cammino, ";", passiCammino);
	printf("Il cammino ha %i passi\n", numPassi);
	// char *segmento = strtok(cammino, ";");
	// // Salto la prima stazione tanto non mi serve
	// segmento = strtok(NULL, ";");
	char *previousSegment = NULL;

	printf("Il treno %lu e' partito \n", numeroTreno);

	// while (segmento != NULL) {
	// Salto la prima stazione perche non mi serve
	for (int i = 1; i < numPassi; ++i){

		char *segmento = malloc(10);
		strcpy(segmento, passiCammino[i]);

		if (i > 0) {
			previousSegment = malloc(10);
			strcpy(previousSegment, passiCammino[i - 1]);
		}

		// La prima stazione che trovo sara quella di arrivo visto
		// visto che la prima l'ho saltata
		if(isStazione(segmento)){

			printf("Stazione %s\n", segmento);
			liberaSegmento(previousSegment);

		}
		// Allora e un segmento
		else{

			int numeroSegmento = 0;

			getNumeroSegmentoDaStringa(&numeroSegmento, segmento);
			// printf("%s: il numero del segmetno e: %i\n", segmento, numeroSegmento);
			char *segmentoOccupato = malloc(1);
			readSegmento(numeroSegmento, segmentoOccupato);

			liberaSegmento(previousSegment);

			if(takeSegmento(numeroSegmento) == true){

				printf("Il treno %lu ha occupato il segmento %i\n",numeroTreno, numeroSegmento);
				// if (previousSegment != NULL) printf("Il segmento precedente e'%s\n", previousSegment);

			}
			else {
				printf("Il treno %lu si e' bloccato sul segmento %s bloccato\n", numeroTreno, segmento);
			}
			// printf("%c\n", *segmentoOccupato);

		}
		sleep(1);

	}

	printf("Il treno %lu ha finito\n", numeroTreno);

}

void liberaSegmento(char *segmento){

	// Se il segmento passato e valido allora li libero
	if (segmento != NULL) {
		int numeroSegmento = 0;
		getNumeroSegmentoDaStringa(&numeroSegmento, segmento);
		freeSegmento(numeroSegmento);
		// printf("Ho liberato il segmento %s\n", segmento);
	}

}
//Helpers

// Divide una stringa
int splitString(char *toSplit, const char *delimiter, char *tokens[10]){

	int i = 0;
	char *token = strtok(toSplit, delimiter);

	while(token != NULL){
		tokens[i] = malloc(50);
		strcpy(tokens[i], token);
		// printf("Questo e da tokens %s\n", tokens[i]);
		i++;
		token = strtok(NULL, delimiter);
	}

	return i;

}

bool isStazione(char *string) {

	return *string == 'S';

}
