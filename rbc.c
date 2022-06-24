#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>

#include "segmentiManager.h"
#include "socketHelper.h"
#include "log.h"

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10
#define NUMERO_TRATTE 16
#define NUMERO_TRENI 5
#define NUMERO_STAZIONI 8
#define SERVER_REGISTRO "serveRegistro"
#define DEBUG true

void gestisciRichiesta(int, char*);
void gestisciOccupazione(int, char*);
void gestisciVerificaSegmentoLibero(int, char *);
void gestisciStazione(int, char*);
void gestisciRilascio(int, char *);
bool isRichiestaOccupazione(char *);
bool isRichiestaVerificaSegmentoLibero(char *);
bool isRichiestaStazione(char *);
bool isRichiestaRilascio(char *);
char* getPercorsi(int, char *);
void setup();

bool segmenti[NUMERO_TRATTE];
int stazioni[NUMERO_STAZIONI];
FILE* logFile;

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

	setup();

	char *mappa = argv[1];
	
	int registroFd = creaConnessioneAServer(SERVER_REGISTRO);
	
	char *percorsi = getPercorsi(registroFd, mappa);

	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);

	int clientConnessi = 0;
	int nfds = 10;

	struct pollfd *fds = calloc(nfds, sizeof(struct pollfd));

	fds[0].fd = serverFd;
	fds[0].events = POLLIN;

	for (int i = 1; i < nfds; ++i)
	{

		fds[i].fd = -1;
		fds[i].events = POLLIN;

	}

	while(1) {

		poll(fds, nfds, -1);

		for (int i = 0; i < nfds; ++i)
		{
			
			if (fds[i].revents == 0) continue;

			if (fds[i].fd == serverFd){

				// Nuova connessione
				fds[1 + clientConnessi].fd = accettaRichiesta(serverFd);
				// printf("creato il socket con fd: %i\n", fds[1 + clientConnessi].fd);
				fds[1 + clientConnessi++].events = POLLIN;
				if(DEBUG)
					printf("Connesso un nuovo client\n");

			}
			else {

				// Si tratta di un client che fa una richiesta
				char * buffer = malloc(10);
				int size = recv(fds[i].fd, buffer, 10, 0);

				// Se il il client ha chiuso il socket (o per altro, tipo erorri)
				if (size == 0) {
					close(fds[i].fd);
					fds[i].fd = -1;
					continue;
				}

				gestisciRichiesta(fds[i].fd, buffer);

			}

		}

	}

	close(serverFd);

	return 0;

}

void setup() {
 
	// Inizializzo lo stato dei segmenti
	for (int i = 0; i < NUMERO_TRATTE; ++i){
		segmenti[i] = false;
	}

	// Inizializzo lo stato delle stazioni
	for (int i = 0; i < NUMERO_STAZIONI; ++i){
		stazioni[i] = 0;
	}

	logFile = creaFileLogServerRBC();

	logRBCStazioneConcessa("test", logFile);

	// fclose(logFile);

}

char* getPercorsi(int registroFd, char * mappa){

	char *message = malloc(10);

	// Il messaggio puo' essere A1 o A2 (sarebbe All1, per richiedere tutta la mappa1 o 
	// All2 per richiedere tutta la mappa1). Per conoscere il numero della mappa mi basta prendere
	// il sesto carattere della stringa MAPPAX.
	sprintf(message, "A%c", mappa[5]);

	send(registroFd, message, 10, 0);

	recv(registroFd, message, 10, 0);

	return message;

}

void gestisciRichiesta(int clientFd, char *messaggio) {

	if(DEBUG)
		printf("letto: %s\n", messaggio);

    if (isRichiestaOccupazione(messaggio) == true){

    	gestisciOccupazione(clientFd, messaggio);

    }
    else if (isRichiestaVerificaSegmentoLibero(messaggio) == true){

    	// gestisciRilascio(clientFd, messaggio);
    	gestisciVerificaSegmentoLibero(clientFd, messaggio);


    }
    else if(isRichiestaStazione(messaggio) == true){

    	// Acetta sempre la richiesta
    	gestisciStazione(clientFd, messaggio);

    }
    else if(isRichiestaRilascio(messaggio) == true) {

		gestisciRilascio(clientFd, messaggio);

    }

}

void gestisciRilascio (int clientFd, char* messaggio){

	// Salto la prima lettera
	messaggio++;
	//libero il segmento
	segmenti[strtol(messaggio, NULL, 10) - 1] = false;

	if(DEBUG)
		printf("segmento liberato %lu\n", strtol(messaggio, NULL, 10));

}

void gestisciStazione(int clientFd, char* messaggio) {

	if(DEBUG)
		printf("stazione concessa: %s\n", messaggio);
	send(clientFd, "1", 10, 0);
	logRBCStazioneConcessa(messaggio, logFile);

}

void gestisciOccupazione(int clientFd, char* messaggio) {

	messaggio++;
	segmenti[strtol(messaggio, NULL, 10) - 1] = true;

}

void gestisciVerificaSegmentoLibero(int clientFd, char *messaggio) {

	// Salto la prima lettera
	messaggio++;

	int numeroSegmento = strtol(messaggio, NULL, 10);
	if (segmenti[numeroSegmento - 1] == false){
		logRBCSegmentoConcesso(numeroSegmento, logFile);
		*messaggio = '1';
	}
	else {
		logRBCSegmentoNegato(numeroSegmento, logFile);
		*messaggio = '0';
	}

	send(clientFd, messaggio, 10, 0);

}

bool isRichiestaOccupazione(char *richiesta) {

	return *richiesta == 'O';

}

bool isRichiestaVerificaSegmentoLibero(char *richiesta) {

	return *richiesta == 'L';

}

bool isRichiestaStazione(char *richiesta) {

	return *richiesta == 'S';

}

bool isRichiestaRilascio(char *richiesta) {

	return *richiesta == 'R';

}