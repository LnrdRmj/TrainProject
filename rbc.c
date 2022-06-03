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

	while(1) {

		printf("aspetto connessioni\n");
		int clientFd = accettaRichiesta(serverFd);

	}

	close(serverFd);

	return 0;

}