#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define NUMERO_TRENI 5
#define MAX_LUNGHEZZA_CAMMINO 100
#define SERVER_NAME "serveRegistro"

void stampaMappa (const char mappa[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10]);
void getCammino(char *, int, char *);

int main() {

  int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

  struct sockaddr_un serverAddress;

  serverAddress.sun_family = AF_UNIX;
  strcpy(serverAddress.sun_path, SERVER_NAME);
  unlink(SERVER_NAME);

  bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  listen(serverFd, NUMERO_TRENI);

  int cont = 0;

  while(1) {

    struct sockaddr_un clientAddress;
    int sizeClient = sizeof(clientAddress);
    int clientFd = accept(serverFd, (struct sockaddr*)&clientAddress, &sizeClient);

    printf("Ricevuta una nuova connessione\n");

    char buffer[1024];
    read(clientFd, buffer, 1024);
    int numeroTreno = atoi(&buffer[0]);
    printf("numero treno %i\n", numeroTreno);
    // printf("%i\n", numeroTreno);
    
    char *mappa = malloc(7);
    memcpy(mappa, &buffer[1], 6);
    mappa[6] = '\0';
    printf("La mappa da usare e %s\n", mappa);

    char *cammino = malloc( MAX_LUNGHEZZA_CAMMINO );
    getCammino(cammino, numeroTreno, mappa);
    printf("%s\n", cammino);
    write(clientFd, cammino, strlen(cammino) + 1);

    close(clientFd);

  }

  close(serverFd);

  return 0;

}

void getCammino(char* cammino, int treno, char* mappa){

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

void stampaMappa(const char mappa[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10]){

  for (size_t i = 0; i < NUMERO_TRENI; i++) {

    for (size_t j = 0; j < MAX_LUNGHEZZA_CAMMINO; j++) {

      printf("%s ", mappa[i][j]);

    }

    printf("\n");

  }

}
