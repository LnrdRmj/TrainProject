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
	
	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);

	while(1) {

		int clientFd = accettaRichiesta(serverFd);

	}

	return 0;

}