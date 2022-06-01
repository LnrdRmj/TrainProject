#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

void test(void);
FILE * creaFileLogTreno(int , FILE *);
void logStatoTreno(char*, char*, FILE*);
void logInizioViaggio(char*, FILE*);
void logFineViaggio(char*, FILE*);
char* dataOraAttuali();

void test() {
	printf("test 4\n");
}

FILE * creaFileLogTreno(int numeroTreno, FILE *fileLog) {

	char* nomeLogFile = malloc(10);
	asprintf(&nomeLogFile, "log/T%i.log", numeroTreno);

	return fopen(nomeLogFile, "w");

}

void logInizioViaggio(char *stazioneIniziale, FILE *logFile) {

	fprintf(logFile, "Sono partito dalla stazione %s [%s]\n",
			stazioneIniziale,
			dataOraAttuali());

}

void logFineViaggio(char *stazioneFinale, FILE *logFile) {

	fprintf(logFile, "Sono arrivato alla stazione %s \t [%s]\n",
			stazioneFinale,
			dataOraAttuali());

}

void logStatoTreno(char *posizioneCorrente, char* prossimaPosizione, FILE *logFile) {

	fprintf(logFile, "Sono in %s. (Prossimo passo %s) \t [%s]\n",
			posizioneCorrente,
			prossimaPosizione,
			dataOraAttuali());

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
