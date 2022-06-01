#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

void test(void);
FILE * creaFileLogTreno(int , FILE *);
void logStatoTreno(char*, char*, FILE*);
void logInizioViaggio(char*, FILE*);
void logFineViaggio(char*, FILE*);

void test() {
	printf("test 4\n");
}

FILE * creaFileLogTreno(int numeroTreno, FILE *fileLog) {

	char* nomeLogFile = malloc(10);
	asprintf(&nomeLogFile, "log/T%i.log", numeroTreno);

	return fopen(nomeLogFile, "w");

}

void logInizioViaggio(char *stazioneIniziale, FILE *logFile) {

	fprintf(logFile, "Sono partito dalla stazione %s\n",
			stazioneIniziale);

}

void logFineViaggio(char *stazioneFinale, FILE *logFile) {

	fprintf(logFile, "Sono arrivato alla stazione %s\n",
			stazioneFinale);

}

void logStatoTreno(char *posizioneCorrente, char* prossimaPosizione, FILE *logFile) {

	char *tmp = malloc(5);
	// Se non c'e una prossima posizione allora 
	if (prossimaPosizione == NULL) {
		strcpy(tmp, "--");
	}
	else {
		strcpy(tmp, prossimaPosizione);	
	}

	printf("%s\n", tmp);

	fprintf(logFile, "Sono in %s. (Prossimo passo %s) data\n",
			posizioneCorrente,
			tmp);

}