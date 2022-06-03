#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10

int createServer(const char*);
int accettaRichiesta(int);

int main(int argc, char const *argv[])
{
	
	int serverFd = createServer(SERVER_RBC_NAME);

	while(1) {

		int clientFd = accettaRichiesta(serverFd);



	}

	return 0;

}

int accettaRichiesta(int serverFd) {

	struct sockaddr_un clientAddress;
    int sizeClient = sizeof(clientAddress);
    return accept(serverFd, (struct sockaddr*)&clientAddress, &sizeClient);

}

int createServer(const char* serverName) {

	int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un serverAddress;

	serverAddress.sun_family = AF_UNIX;
	strcpy(serverAddress.sun_path, serverName);
	unlink(serverName);

	bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(serverFd, SERVER_QUEUE_LENGTH);

	return serverFd;

}