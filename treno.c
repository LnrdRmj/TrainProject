#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SERVER_NAME "serveRegistro"

void getCammino(char *);

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[0];

  // printf("Sono il treno numero %s\n", numTreno);

  getCammino(numTreno);

  exit(0);
  return 0;

}

void getCammino(char *numTreno){

  struct sockaddr_un registro;
  registro.sun_family = AF_UNIX;
  strcpy(registro.sun_path, SERVER_NAME);

  int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

  int result;
  do{
    int serverLen = sizeof(registro);
    result  = connect(serverFd, (struct sockaddr*) &registro, serverLen);
    if(result == -1) sleep(1);
  }while(result == -1);

  // printf("Treno scrive al server\n");
  write(serverFd, numTreno, strlen(numTreno) + 1);

  char *cammino;
  read(serverFd, cammino, 1024);
  printf("Il cammino del treno %s e': %s\n", numTreno, cammino);

  close(serverFd);

}
