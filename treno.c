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

	char *segmento = strtok(cammino, ";");
	// Salto la prima stazione tanto non mi serve
	segmento = strtok(NULL, ";");
	char *previousSegment = NULL;

	printf("Il treno %lu e' partito \n", numeroTreno);

	while (segmento != NULL) {

		// La prima stazione che trovo sara quella di arrivo visto
		// visto che la prima l'ho saltata
		if(isStazione(segmento)){

			// printf("Stazione %s\n", segmento);
			segmento = strtok(NULL, ";");
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

				previousSegment = malloc(5);
				strcpy(previousSegment, segmento);
				segmento = strtok(NULL, ";");

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

bool isStazione(char *string) {

	return *string == 'S';

}
