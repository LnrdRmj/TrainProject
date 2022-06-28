#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include "socketHelper.h"

#define NUMERO_TRENI 5
#define MAX_LUNGHEZZA_CAMMINO 100
#define SERVER_NAME "serveRegistro"

void getCammino(char*, int, char*);
void gestisciRBC(int, char[1024]);
void sendMappaIntera(int, char);
void gestisciTreno(int, char [1024]);

char mappa1[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO] = {
    { "S1;MA1;MA2;MA3;MA8;S6" },
    { "S2;MA5;MA6;MA7;MA3;MA8;S6" },
    { "S7;MA13;MA12;MA11;MA10;MA9;S3" },
    { "S4;MA14;MA15;MA16;MA12;S8" },
    { "" },
  };

char mappa2[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO] = {
  { "S2;MA5;MA6;MA7;MA3;MA8;S6" },
  { "S3;MA9;MA10;MA11;MA12;S8" },
  { "S4;MA14;MA15;MA16;MA12;S8" },
  { "S6;MA8;MA3;MA2;MA1;S1" },
  { "S5;MA4;MA3;MA2;MA1;S1" },
};

int main() {

  int serverFd = createServer(SERVER_NAME, NUMERO_TRENI);

  while(1) {

    int clientFd = accettaRichiesta(serverFd);

    // printf("Ricevuta una nuova connessione\n");

    char buffer[1024];
    read(clientFd, buffer, 1024);

    if (buffer[0] == 'A'){

      gestisciRBC(clientFd, buffer);

    }else{

      gestisciTreno(clientFd, buffer);

    }

  }

  close(serverFd);

  return 0;

}

void gestisciRBC(int clientFd, char buffer[1024]) {

    sendMappaIntera(clientFd, buffer[1]);

}

void sendMappaIntera(int clientFd, char numeroMappa) {

  char *mappaIntera = malloc(256);
  char *tmp = mappaIntera;

  for (int i = 0; i < NUMERO_TRENI; ++i)
  {

    if (numeroMappa == '1'){
      strcpy(tmp, mappa1[i]);
      tmp += strlen(mappa1[i]);
      *tmp = '/';
      tmp++;
    }
    else if (numeroMappa == '2'){
      strcpy(tmp, mappa2[i]);
      tmp += strlen(mappa2[i]);
      *tmp = '/';
      tmp++;
    }

  }

  printf("mappa intera %s\n", mappaIntera);
  send(clientFd, mappaIntera, 256, 0);

}

void gestisciTreno(int clientFd, char buffer[1024]) {

    int numeroTreno = atoi(&buffer[0]);
    
    char *mappa = malloc(7);
    memcpy(mappa, &buffer[1], 6);
    mappa[6] = '\0';

    char *cammino = malloc( MAX_LUNGHEZZA_CAMMINO );
    getCammino(cammino, numeroTreno, mappa);
    // printf("%s\n", cammino);
    write(clientFd, cammino, strlen(cammino) + 1);

    close(clientFd);

}

void getCammino(char* cammino, int treno, char* mappa){

  int i = 0;

  if (strcmp(mappa, "MAPPA1") == 0){
    while(mappa1[treno][i] != '\0'){
      cammino[i] = mappa1[treno][i];
      ++i;
    }
  }
  else if(strcmp(mappa, "MAPPA2") == 0){
    while(mappa2[treno][i] != '\0'){
      cammino[i] = mappa2[treno][i];
      ++i;
    }
  }

  cammino[i] = '\0';

}
