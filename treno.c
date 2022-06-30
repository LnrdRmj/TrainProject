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
#define DEBUG false
#define SLEEP_TIME 2

// typedef bool (*politicaSegmento) (int, int);
typedef void (*politicaRilascio) (int, int);
typedef bool (*controlloSegmentoLibero) (int, int);

void setup(char*, char*, char*);
char* getCammino(char*);
void startAttraversata();
bool isStazione(char*);
int splitString(char*, const char *, char *[]);
politicaRilascio scegliStrategiaRilascio(char*);
controlloSegmentoLibero stategiaSegmentoIsLibero(char*);
void takeSegmentoRBC (int, int);
bool permessoStazione(int, char *);
void gestisciStazione(char*, char*);
bool gestisciSegmento(char*, char*, char*);

FILE* logFile;
long numeroTreno;
int serverRBC;
char* cammino;
politicaRilascio rilascioSegmento;
controlloSegmentoLibero isSegmentoLibero;

int main(int argc, char *argv[]) {

	// argv[0] e' il nome del programma
	char* numTreno = argv[1]; 	// Il primo argomento è il numero del treno
	char* modalita = argv[2];		// Il secondo invece e' la modalita'
	char* mappa = argv[3];		// Il terzo indica la mappa da utilizzare

	setup(numTreno, modalita, mappa);

	// startAttraversata 
	startAttraversata(modalita);

	fclose(logFile);

	return 0;

}

void setup(char* numTreno, char* modalita, char* mappa){

	// Converto la stringa numTreno in long
	numeroTreno = strtol(numTreno, NULL, 10);

	// Recupera il cammino del treno dal processo registro
	cammino = getCammino(mappa);

	// Inizializzo il logfile
	logFile = creaFileLogTreno(numeroTreno);

	// Inizializzo il serverRBC se in ETCS2
	serverRBC = -1;
	if(strcmp(modalita, ETCS2) == 0)
		serverRBC = creaConnessioneAServer(SERVER_RBC);

	// Queste sono le "politiche" che cambiano in base alla modalita scelta (ETCS1/2)
	rilascioSegmento = scegliStrategiaRilascio(modalita);
	isSegmentoLibero = stategiaSegmentoIsLibero(modalita);

}

char* getCammino(char *mappa){

	// Creo la connesione al server registro
	int serverFd = creaConnessioneAServer(SERVER_REGISTRO);

	char *buffer = malloc(10);
	sprintf(buffer, "%li", numeroTreno); 

	// Scrivo al server registro che treno sono
	char *dataPerServer = strcat(buffer, mappa);
	write(serverFd, dataPerServer, strlen(dataPerServer) + 1);

	// Leggo dal server registro il cammino 
	// Il registro risponde con un messaggio del tipo:
	// S1;MA1;MA2;MA3;MA4;MA5;MA6;S2
	char *cammino = malloc(100);
	read(serverFd, cammino, 100);
	if(DEBUG) printf("Il cammino del treno %lu e': %s\n", numeroTreno, cammino);

	close(serverFd);

	return cammino;

}
// lnumeroTreno
void startAttraversata(){

	printf("Il treno %li parte\n", numeroTreno);

	// Questo array conterra' il percorso sottoforma di array
	char *passiCammino[10];

	// ottengo i passi dell'attraversata
	int numPassi = splitString(cammino, ";", passiCammino);

	char *previousSegment = NULL;

	if (DEBUG)
		printf("Il treno %lu e' partito \n", numeroTreno);

	// Cicla tutti i passi (quindi segmenti e stazioni) del percorso
	for (int i = 0; i < numPassi; ++i){

		// In realta il segmento puo essere anche una stazione
		char *segmento = malloc(10);
		strcpy(segmento, passiCammino[i]);

		if (i > 0) {
			previousSegment = malloc(10);
			strcpy(previousSegment, passiCammino[i - 1]);
		}

		// Se si tratta di una stazione
		if(isStazione(segmento)){

			if (i == 0) logInizioViaggio(segmento, logFile);
			else 		logFineViaggio(segmento, logFile);

			gestisciStazione(segmento, previousSegment);

		}
		// Se si tratta di un segmento
		else{
			
			bool result = gestisciSegmento(segmento, previousSegment, passiCammino[i + 1]);
			// Se si blocca allora deve rimanere sullo stesso segmento e riprovare
			// a riprenderlo quindi diminuisco di uno l'indice
			if (result == false)
				--i;

		}

		sleep(SLEEP_TIME);

	}

	close(serverRBC);
	printf("Il treno %lu ha finito\n", numeroTreno);

}

void gestisciStazione(char* segmento, char* previousSegment) {

	if (serverRBC != -1) permessoStazione(serverRBC, segmento);

	if(DEBUG)
		printf("Stazione %s\n", segmento);

	if (previousSegment != NULL)
		rilascioSegmento(getNumeroSegmentoDaStringa(previousSegment), serverRBC);

}

// Si occupa di gestire il passaggio al prossimo segmento (se possibile)
// Ritorna true se il treno e' passato al segmento/stazione successiva, false altrimento
bool gestisciSegmento(char* segmento, char* previousSegment, char* prossimoSegmento) {

	int numeroSegmento = getNumeroSegmentoDaStringa(segmento);

	// Libero se e' il passo precedente era un segmento
	if (!isStazione(previousSegment))
		rilascioSegmento(getNumeroSegmentoDaStringa(previousSegment), serverRBC);

	// Se il segmento richiesto e' libero
	if(isSegmentoLibero(numeroSegmento, serverRBC) == true){

		takeSegmento(numeroSegmento);
		if(DEBUG) printf("Il treno %li ha occupato il segmento %s\n", numeroTreno, segmento);

		if(serverRBC != -1)
			takeSegmentoRBC(numeroSegmento, serverRBC);

		logStatoTreno(segmento, prossimoSegmento, logFile);

		// Il treno puo' continuare il suo cammino
		return true;

	}
	else {

		if(DEBUG) printf("Il treno %li si e' bloccato sul segmento %s\n", numeroTreno, previousSegment);

		logTrenoBloccato(previousSegment, logFile);
		// Il treno si e' bloccato
		return false;

	}

}

// Chiede il permesso di entrare in una stazione al serverRBC
bool permessoStazione(int serverRBC, char *stazione) {

	char *message = malloc(10);
	// Stazione sara una stringa del tipo SX dove X e' un numero tra 1 e 8 (compresi)
	// A me interessa solo il numero quindi
	sprintf(message, "S%i", atoi(++stazione)); //S per Stazione
	send(serverRBC, message, 10, 0);

	recv(serverRBC, message, 10, 0);

	return *message == '1';

}

bool segmentoIsLiberoETCS1(int numeroSegmento, int fantoccio) {

	// Nel caso di ETCS1 ci basta verificare che il segmento abbia "0" nel suo file
	return segmentoIsLibero(numeroSegmento);

}

bool segmentoIsLiberoETCS2(int numeroSegmento, int serverRBC) {

	// Nel caso di ETCS2 dobbiamo contattare il server RBC per controllare un segmento
	char *message = malloc(10);
	sprintf(message, "L%i", numeroSegmento); //L per Libero
	send(serverRBC, message, 10, 0);

	char *response = malloc(10);
    recv(serverRBC, response, 10, 0);

	return *response == '1';

}

// Sceglie la strategia usate per verificare se un segmento e' libero in base alla modalita
// Le strategie ritornano true se il segmento e' libero, false altrimenti
controlloSegmentoLibero stategiaSegmentoIsLibero(char *modalita){

	if (strcmp(modalita, "ETCS1") == 0) {
		return segmentoIsLiberoETCS1;
	}
	else if(strcmp(modalita, "ETCS2") == 0){
		return segmentoIsLiberoETCS2;
	}

}

// Comunica al serverRBC di occupare un segmento
void takeSegmentoRBC (int numeroSegmento, int serverRBC) {

	char *message = malloc(10);
	sprintf(message, "O%i", numeroSegmento); //O per Occupa
	send(serverRBC, message, 10, 0);

}

void politicaRilascioETCS1(int segmento, int fantoccio) {

	// Nella modalita ETCS1 ci basta scrivere "0" nel file del segmento corrispondente
	freeSegmento(segmento);


}

void politicaRilascioETCS2(int segmento, int serverRBC) {

	// Nella politica ETCS2 dobbiamo continuare a scrivere "0" sul file segmento ma...
	freeSegmento(segmento);

	// ...dobbiamo anche informare il server RBC 
	char *message = malloc(10);
	sprintf(message, "R%i", segmento); //R per "Rilascio"

	send(serverRBC, message, 10, 0);

}

// Sceglie la strategia usate per rilasciare un segmento in base alla modalita
politicaRilascio scegliStrategiaRilascio(char *modalita) {

	if (strcmp(modalita, "ETCS1") == 0) {
		return politicaRilascioETCS1;
	}
	else if(strcmp(modalita, "ETCS2") == 0){
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
		i++;
		token = strtok(NULL, delimiter);
	}

	return i;

}

// Controlla se un passo del cammino e' una stazione oppure no
bool isStazione(char *string) {

	return *string == 'S';

}
