#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "socketHelper.h"

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);

	int pidFigli[10];
	int figli = 0;

	while(1) {

		int clientFd = accettaRichiesta(serverFd);

		if((pidFigli[figli++] = fork()) == 0) {



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

	printf("richiesta gestita\n");

	// while(1) {

	// 	// TODO gestisci le richieste dei client finche non vieni terminato dal padre


	// }

}