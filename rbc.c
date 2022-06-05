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

void gestisciRichiesta(int);
void gestisciRilascio (int, char*);
void gestisciOccupazione(int, char*);
bool isRichiestaOccupazione(char *);

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

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

void gestisciRichiesta(int clientFd) {

	while(1 && clientFd != 0) {

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