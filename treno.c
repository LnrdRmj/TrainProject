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
  // printf("%s\n", dataForServer);
  write(serverFd, dataForServer, strlen(dataForServer) + 1);

  char *cammino = malloc(100);
  read(serverFd, cammino, 100);
  // printf("Il cammino del treno %s e': %s\n", numTreno, cammino);

  startJourney(cammino, lnumeroTreno);

  close(serverFd);

}

void startJourney(char * cammino, long numeroTreno){

	char *segmento = strtok(cammino, ";");
	segmento = strtok(NULL, ";"); // Salto la prima stazione tanto non mi serve

	while (segmento != NULL) {

		// La prima stazione che trovo sara quella di arrivo visto
		// visto che la prima l'ho saltata
		if(isStazione(segmento)){

			printf("Stazione %s\n", segmento);

		}
		// Allora e un segmento
		else{

			int numeroSegmento = 0;

			getNumeroSegmentoDaStringa(&numeroSegmento, segmento);
			// printf("%s: il numero del segmetno e: %i\n", segmento, numeroSegmento);
			char *segmentoOccupato = malloc(1);
			readSegmento(numeroSegmento, segmentoOccupato);
			printf("%c\n", *segmentoOccupato);

		}
		segmento = strtok(NULL, ";");
		sleep(1);

	}

}

bool isStazione(char *string) {

	return *string == 'S';

} 