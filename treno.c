#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdbool.h>

#include "segmentiManager.h"
#include "log.h"

#define SERVER_NAME "serveRegistro"
#define PREFISSO_FILE_SEGMENTO "MA"
#define DEBUG false

typedef bool (*politicaSegmento) (int, int);

char* getCammino(long, char *);
void startJourney(char *, long, FILE *, char*);
bool isStazione(char *);
void liberaSegmento(char *);
int splitString(char *, const char *, char *[]);
politicaSegmento scegliStrategia(char *);

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[1];
  char *mode = argv[2];
  char *mappa = argv[3];

  // Converto la stringa in long
  char *tmp;
  long lnumeroTreno = strtol(numTreno, &tmp, 10);

  FILE *logFile = creaFileLogTreno(lnumeroTreno, logFile);

  char *cammino = getCammino(lnumeroTreno, mappa);

  // printf("Cammino %s\n", cammino);

  startJourney(cammino, lnumeroTreno, logFile, mode);

  return 0;

}

char* getCammino(long lnumeroTreno, char *mappa){

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

  // char *tmp;
  // long lnumeroTreno = strtol(numTreno, &tmp, 10);

  char *buffer;
  asprintf(&buffer, "%i", lnumeroTreno); 

  char *dataForServer = strcat(buffer, mappa);
  write(serverFd, dataForServer, strlen(dataForServer) + 1);

  char *cammino = malloc(100);
  read(serverFd, cammino, 100);
  // printf("Il cammino del treno %s e': %s\n", numTreno, cammino);

  close(serverFd);

  return cammino;

}

void startJourney(char * cammino, long numeroTreno, FILE *logFile, char* mode){

	// Questo array conterra' il percorso sottoforma di array
	char *passiCammino[10];

	int numPassi = splitString(cammino, ";", passiCammino);
	char *previousSegment = NULL;

	if (DEBUG)
		printf("Il treno %lu e' partito \n", numeroTreno);

	politicaSegmento takeSegmento = scegliStrategia(mode);  

	for (int i = 0; i < numPassi; ++i){

		char *segmento = malloc(10);
		strcpy(segmento, passiCammino[i]);

		if (i > 0) {
			previousSegment = malloc(10);
			strcpy(previousSegment, passiCammino[i - 1]);
		}

		if(isStazione(segmento)){

			if (i == 0) {
				logInizioViaggio(segmento, logFile);
			}
			else{
				logFineViaggio(segmento, logFile);	
			}

			if(DEBUG)
				printf("Stazione %s\n", segmento);

			liberaSegmento(previousSegment);

		}
		// Allora e' un segmento
		else{

			logStatoTreno(segmento, passiCammino[i + 1], logFile);

			int numeroSegmento = getNumeroSegmentoDaStringa(segmento);
			// printf("%s: il numero del segmetno e: %i\n", segmento, numeroSegmento);
			char *segmentoOccupato = malloc(1);
			readSegmento(numeroSegmento, segmentoOccupato);

			// Libero solo i segmenti
			if (!isStazione(previousSegment))
				liberaSegmento(previousSegment);

			if(takeSegmento(numeroSegmento, 0) == true){

				if (DEBUG)
				printf("Il treno %lu ha occupato il segmento %i\n",numeroTreno, numeroSegmento);

			}
			else {
				if (DEBUG)
				printf("Il treno %lu si e' bloccato sul segmento %s bloccato\n", numeroTreno, segmento);
			}

		}
		sleep(1);

	}

	printf("Il treno %lu ha finito\n", numeroTreno);

}

bool takeSegmentETCS1(int numeroSegmento, int fantoccio) {

	printf("politica ETCS1\n");
	return true;

}

bool takeSemgneETCS2(int numeroSegmento, int RBC) {

	printf("politica ETCS2\n");
	return true;

}

politicaSegmento scegliStrategia(char *mode) {

	if (strcmp(mode, "ETCS1") == 0) {
		return takeSegmentETCS1;
	}
	else if(strcmp(mode, "ETCS2") == 0){
		return takeSemgneETCS2;
	}

}

void liberaSegmento(char *segmento){

	// Se il segmento passato e valido allora li libero
	if (segmento != NULL) {
		int numeroSegmento = getNumeroSegmentoDaStringa(segmento);
		freeSegmento(numeroSegmento);
		printf("liberato %i\n", numeroSegmento);
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
