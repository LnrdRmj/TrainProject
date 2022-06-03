#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "socketHelper.h"

int accettaRichiesta(int);
int createServer(const char*, int);

int accettaRichiesta(int serverFd) {

	struct sockaddr_un clientAddress;
    int sizeClient = sizeof(clientAddress);
    return accept(serverFd, (struct sockaddr*)&clientAddress, &sizeClient);

}

int createServer(const char* serverName, int lunghezzaCodaServer) {

	int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un serverAddress;

	serverAddress.sun_family = AF_UNIX;
	strcpy(serverAddress.sun_path, serverName);
	unlink(serverName);

	bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	listen(serverFd, lunghezzaCodaServer);

	return serverFd;

}