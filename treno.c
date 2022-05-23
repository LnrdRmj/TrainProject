#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SERVER_NAME "serveRegistro"

void getCammino(char *, char *);
void startJourney(char *);

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[1];
  char *mappa = argv[2];

  getCammino(numTreno, mappa);

  return 0;

}

void getCammino(char *numTreno, char *mappa){

  struct sockaddr_un registro;
  registro.sun_family = AF_UNIX;
  strcpy(registro.sun_path, SERVER_NAME);

  int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

  int result;
  do{
    int serverLen = sizeof(registro);
    result = connect(serverFd, (struct sockaddr*) &registro, serverLen);
    if(result == -1) {
    	sleep(1);
    }
  }while(result == -1);

  char *dataForServer = strcat(numTreno, mappa);
  write(serverFd, dataForServer, strlen(dataForServer) + 1);

  char *cammino = malloc(100);
  read(serverFd, cammino, 100);
  printf("Il cammino del treno %s e': %s\n", numTreno, cammino);

  startJourney(cammino);

  close(serverFd);

}

void startJourney(char * cammino){

	char *passo = strtok(cammino, ";");

	while (passo != NULL) {

		printf("%s\n", passo);
		passo = strtok(NULL, ";");

	}

}
