#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

FILE* creaFileLogTreno(int);
void logStatoTreno(char*, char*, FILE*);
void logInizioViaggio(char*, FILE*);
void logFineViaggio(char*, FILE*);
FILE* creaFileLogServerRBC();
void logRBCStazioneConcessa(char*, FILE*);
void logRBCSegmentoConcesso(int, FILE*);
void logRBCSegmentoNegato(int, FILE*);
char* dataOraAttuali();

// Log per i treni
FILE* creaFileLogTreno(int numeroTreno) {

	char* nomeLogFile = malloc(10);
	asprintf(&nomeLogFile, "logs/T%i.log", numeroTreno);

	return fopen(nomeLogFile, "w");

}

void logInizioViaggio(char* stazioneIniziale, FILE* logFile) {

	fprintf(logFile, "Sono partito dalla stazione %s [%s]\n",
			stazioneIniziale,
			dataOraAttuali());
	fflush(logFile);

}

void logFineViaggio(char* stazioneFinale, FILE* logFile) {

	fprintf(logFile, "Sono arrivato alla stazione %s \t [%s]\n",
			stazioneFinale,
			dataOraAttuali());
	fflush(logFile);

}

void logStatoTreno(char* posizioneCorrente, char* prossimaPosizione, FILE* logFile) {

	fprintf(logFile, "Sono in %s. (Prossimo passo %s) \t [%s]\n",
			posizioneCorrente,
			prossimaPosizione,
			dataOraAttuali());
	fflush(logFile);

}

// Log per il server RBC
FILE* creaFileLogServerRBC() {

	return fopen("logs/RBC.log", "w");

}

void logRBCStazioneConcessa(char* stazione, FILE* logFile) {

	fprintf(logFile, "Concesso l'accesso alla stazione %s-esima ad un treno [%s]",
			stazione,
			dataOraAttuali());
	fflush(logFile);

}

void logRBCSegmentoConcesso(int segmento, FILE* logFile) {

	fprintf(logFile, "Concesso il segmento %i-esimo ad un treno [%s]",
			segmento,
			dataOraAttuali());
	fflush(logFile);

}

void logRBCSegmentoNegato(int segmento, FILE* logFile) {

	fprintf(logFile, "Negato il segmento %i-esimo ad un treno [%s]",
			segmento,
			dataOraAttuali());
	fflush(logFile);

}

char* dataOraAttuali() {

	char *dataOra = malloc(100);

	time_t rawtime;
	struct tm * timeInfo;

	time(&rawtime);
	timeInfo = localtime(&rawtime);

	sprintf(dataOra, "%d/%d/%d %d:%d:%d",
		timeInfo->tm_mday,
		timeInfo->tm_mon + 1,
		timeInfo->tm_year + 1900,
		timeInfo->tm_hour,
		timeInfo->tm_min,
		timeInfo->tm_sec);

	return dataOra;

}
