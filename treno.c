#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdbool.h>

#include "segmentiManager.h"
#include "log.h"
#include "socketHelper.h"

#define SERVER_REGISTRO "serveRegistro"
#define SERVER_RBC "serverRBC"
#define ETCS1 "ETCS1"
#define ETCS2 "ETCS2"
#define PREFISSO_FILE_SEGMENTO "MA"
#define DEBUG true

typedef bool (*politicaSegmento) (int, int);
typedef void (*politicaRilascio) (int, int);

char* getCammino(long, char *);
void startJourney(char *, long, FILE *, char*);
bool isStazione(char *);
// void liberaSegmento(char *);
int splitString(char *, const char *, char *[]);
politicaSegmento scegliStrategia(char *);
politicaRilascio scegliStrategiaRilascio(char *);
// int creaConnessioneAServer(const char*);

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

	int serverFd = creaConnessioneAServer(SERVER_REGISTRO);

	char *buffer;
	asprintf(&buffer, "%i", lnumeroTreno); 

	char *dataForServer = strcat(buffer, mappa);
	write(serverFd, dataForServer, strlen(dataForServer) + 1);

	char *cammino = malloc(100);
	read(serverFd, cammino, 100);
	if(DEBUG) printf("Il cammino del treno %lu e': %s\n", lnumeroTreno, cammino);

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

	int serverRBC = -1;
	if(strcmp(mode, ETCS2) == 0)
		serverRBC = creaConnessioneAServer(SERVER_RBC);

	politicaSegmento takeSegmento = scegliStrategia(mode);
	politicaRilascio rilascioSegmento = scegliStrategiaRilascio(mode);

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

			if (previousSegment != NULL)
				rilascioSegmento(getNumeroSegmentoDaStringa(previousSegment), serverRBC);

		}
		// Allora e' un segmento
		else{

			logStatoTreno(segmento, passiCammino[i + 1], logFile);

			int numeroSegmento = getNumeroSegmentoDaStringa(segmento);
			// printf("%s: il numero del segmetno e: %i\n", segmento, numeroSegmento);

			// Libero se e' un segmento
			if (!isStazione(previousSegment))
				rilascioSegmento(getNumeroSegmentoDaStringa(previousSegment), serverRBC);

			if(takeSegmento(numeroSegmento, serverRBC) == true){

				if (DEBUG)
				printf("Il treno %lu ha occupato il segmento %i\n",numeroTreno, numeroSegmento);

			}
			else {
				// Se si blocca allora deve rimanere sullo stesso segmento
				// quindi diminuisco di uno l'indice
				--i;
				if (true)
				printf("Il treno %lu si e' bloccato sul segmento %s bloccato\n", numeroTreno, segmento);
			}

		}
		sleep(1);

	}

	printf("Il treno %lu ha finito\n", numeroTreno);

}

bool politicaETCS1(int numeroSegmento, int fantoccio) {

	return takeSegmento(numeroSegmento);

}

bool politicaETCS2(int numeroSegmento, int serverRBC) {

	// printf("Sto prendendo un segmento\n");
	char *message = malloc(3);
	sprintf(message, "O%i\0", numeroSegmento); //O per Occupa
	printf("messagio occupazione %s\n", message);
	send(serverRBC, message, strlen(message) + 1, 0);

	char *response = malloc(1);
	printf("aspett response occupazione\n");
    recv(serverRBC, response, 1, 0);
    printf("Response da RBC %s\n", response);

	return *response == '1';
	return true;

}

politicaSegmento scegliStrategia(char *mode) {

	if (strcmp(mode, "ETCS1") == 0) {
		return politicaETCS1;
	}
	else if(strcmp(mode, "ETCS2") == 0){
		return politicaETCS2;
	}

}

void politicaRilascioETCS1(int segmento, int fantoccio) {

	freeSegmento(segmento);

}

void politicaRilascioETCS2(int segmento, int serverRBC) {

	char message[10]; //L per "Libera"
	sprintf(message, "L%i\0", segmento);

	printf("messagio rilascio %s\n", message);	
	send(serverRBC, message, strlen(message) + 1, 0);
	// printf("mandato il rilascio\n");

	return;
}

politicaRilascio scegliStrategiaRilascio(char *mode) {

	if (strcmp(mode, "ETCS1") == 0) {
		return politicaRilascioETCS1;
	}
	else if(strcmp(mode, "ETCS2") == 0){
		return politicaRilascioETCS2;
	}

}

//Helpers

// Divide una stringa dato un delimitatore
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
