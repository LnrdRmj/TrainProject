#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

FILE* creaFileLogTreno(int);
void logStatoTreno(char*, char*, FILE*);
void logInizioViaggio(char*, FILE*);
void logFineViaggio(char*, FILE*);
void logTrenoBloccato(char*, FILE*);
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

	fprintf(logFile, "[%s] - Sono partito dalla stazione %s\n",
			dataOraAttuali(),
			stazioneIniziale);
	fflush(logFile);

}

void logFineViaggio(char* stazioneFinale, FILE* logFile) {

	fprintf(logFile, "[%s] - Sono arrivato alla stazione %s\n",
			dataOraAttuali(),
			stazioneFinale);
	fflush(logFile);

}

void logStatoTreno(char* posizioneCorrente, char* prossimaPosizione, FILE* logFile) {

	fprintf(logFile, "[%s] - Sono in %s. (Prossimo passo %s)\n",
			dataOraAttuali(),
			posizioneCorrente,
			prossimaPosizione);
	fflush(logFile);

}

void logTrenoBloccato(char* segmentoCorrente, FILE* logFile) {

	fprintf(logFile, "[%s] - Sono bloccato nel segmento %s\n",
			dataOraAttuali(),
			segmentoCorrente);
	fflush(logFile);

}

// Log per il server RBC
FILE* creaFileLogServerRBC() {

	return fopen("logs/RBC.log", "w");

}

void logRBCStazioneConcessa(char* stazione, FILE* logFile) {

	fprintf(logFile, "[%s] - Concesso l'accesso alla stazione %s-esima ad un treno\n",
			dataOraAttuali(),
			stazione);
	fflush(logFile);

}

void logRBCSegmentoConcesso(int segmento, FILE* logFile) {

	fprintf(logFile, "[%s] - Concesso il segmento %i-esimo ad un treno\n",
			dataOraAttuali(),
			segmento);
	fflush(logFile);

}

void logRBCSegmentoNegato(int segmento, FILE* logFile) {

	fprintf(logFile, "[%s] - Negato il segmento %i-esimo ad un treno \n",
			dataOraAttuali(),
			segmento);
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
