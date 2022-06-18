#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>

#include "socketHelper.h"

int accettaRichiesta(int);
int createServer(const char*, int);
int creaConnessioneAServer(const char*);

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

int creaConnessioneAServer(const char* serverName) {

	struct sockaddr_un registro;
	registro.sun_family = AF_UNIX;
	strcpy(registro.sun_path, serverName);

	int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

	int result;
	do{
		int serverLen = sizeof(registro);
		result = connect(serverFd, (struct sockaddr*) &registro, serverLen);
		if(result == -1) {
			printf("coneessione a %s non riuscita\n", serverName);
			sleep(1);
		}
	}while(result == -1);

	return serverFd;

}