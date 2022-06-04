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

	while(1) {

		char messaggio[10];
	    read(clientFd, messaggio, 10);

	    printf("RBC - Ho letto %s\n", messaggio);

	    if (isRichiestaOccupazione(&messaggio) == true){

	    	gestisciOccupazione(clientFd, messaggio);

	    }
	    else {

	    	gestisciRilascio(clientFd, messaggio);
	    	printf("finisce\n");

	    }

	}

}

void gestisciRilascio (int clientFd, char* messaggio){

	// Salto la prima lettera
	messaggio++;
	printf("%lu\n", strtol(messaggio, NULL, 10));
	freeSegmento(strtol(messaggio, NULL, 10));

}

void gestisciOccupazione(int clientFd, char* messaggio) {

	char response;

	// Salto la prima lettera
	messaggio++;

	if (takeSegmento(strtol(messaggio, NULL, 10))) {

		printf("segmento preso\n");
		response = '1';

	}
	else {

		printf("segmento non preso\n");
		response = '0';

	}

	write(clientFd, &response, 1);

}

bool isRichiestaOccupazione(char *richiesta) {

	return *richiesta == 'O';

}