#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>

#include "segmentiManager.h"
#include "socketHelper.h"

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10
#define NUMERO_TRATTE 16
#define NUMERO_TRENI 5
#define SERVER_REGISTRO "serveRegistro"

void gestisciRichiesta(int);
void gestisciRilascio (int, char*);
void gestisciOccupazione(int, char*);
bool isRichiestaOccupazione(char *);
char* getPercorsi(int, char *);

bool segmenti[NUMERO_TRATTE];

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

	for (int i = 0; i < NUMERO_TRATTE; ++i){
		segmenti[i] = false;
	}

	char *mappa = argv[1];
	
	int registroFd = creaConnessioneAServer(SERVER_REGISTRO);

	char *percorsi = getPercorsi(registroFd, mappa);

	return 1;

	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);

	int pidFigli[10];
	int figli = 0;

	while(1) {

		int clientFd = accettaRichiesta(serverFd);
		printf("nuova connessione a RBC\n");

		if((pidFigli[figli++] = fork()) == 0) {

			gestisciRichiesta(clientFd);

		}

		for (int i = 0; i < 10; i++){

			printf("%i ", pidFigli[i]);

		}

		printf("\n");

	}

	close(serverFd);

	return 0;

}

char* getPercorsi(int registroFd, char * mappa){


	char * message = malloc(10);

	sprintf(message, "A%c", mappa[5]);
	printf("%s\n", message);

	send(registroFd, message, 10, 0);

	recv(registroFd, message, 1024, 0);
	printf("%s\n", message);

}

void gestisciRichiesta(int clientFd) {

	while(1 && clientFd != -1) {

		char *messaggio = malloc(10);
		printf("Aspetto una nuoca richiesta\n");
	    recv(clientFd, messaggio, 10, 0);

	    printf("RBC - Ho letto %s\n", messaggio);

	    if (isRichiestaOccupazione(messaggio) == true){

	    	gestisciOccupazione(clientFd, messaggio);

	    }
	    else {

	    	gestisciRilascio(clientFd, messaggio);

	    }

	}

}

void gestisciRilascio (int clientFd, char* messaggio){

	// Salto la prima lettera
	messaggio++;
	freeSegmento(strtol(messaggio, NULL, 10));
	printf("segmento liberato %lu\n", strtol(messaggio, NULL, 10));

}

void gestisciOccupazione(int clientFd, char* messaggio) {

	char *response = malloc(10);

	// Salto la prima lettera
	messaggio++;

	if (takeSegmento(strtol(messaggio, NULL, 10))) {

		printf("segmento %lu preso\n", strtol(messaggio, NULL, 10));
		*response = '1';

	}
	else {

		printf("segmento %lu non preso\n", strtol(messaggio, NULL, 10));
		*response = '0';

	}

	printf("server - Mando %s\n", response);
	send(clientFd, response, 10, 0);

}

bool isRichiestaOccupazione(char *richiesta) {

	return *richiesta == 'O';

}

// char* getCamminoTreno(int registro, int numeroTreno, char *mappa){

// 	char *buffer;
// 	sprintf(buffer, "%i", numeroTreno); 

// 	char *dataForServer = strcat(buffer, mappa);
// 	write(registro, dataForServer, strlen(dataForServer) + 1);

// 	char *cammino = malloc(100);
// 	read(registro, cammino, 100);

// 	return cammino;

// }