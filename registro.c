#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define NUMERO_TRENI 5
#define MAX_LUNGHEZZA_CAMMINO 10
#define PORT 8000
#define SERVER_NAME "registro"

void stampaMappa (const char mappa[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10]);
void getCammino(char * [], int);

int main() {

  printf("Sono il registro\n");

  int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);

  struct sockaddr_un serverAddress;

  serverAddress.sun_family = AF_UNIX;
  strcpy(serverAddress.sun_path, SERVER_NAME);
  unlink(SERVER_NAME);

  printf("Ho fatto il binding\n");
  bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

  printf("Sto ascoltando sulla porta %i\n", PORT);
  listen(serverFd, NUMERO_TRENI);

  int cont = 0;

  while(cont < 1) {

    struct sockaddr_un clientAddress;
    int clientLen = sizeof(clientAddress);
    int clientFd = accept(serverFd, (struct sockaddr*)&clientAddress, &clientLen);

    printf("Ricevuta una nuova connessione\n");
    close(clientFd);

    cont++;

  }

  close(serverFd);

  return 0;

}

int test = 0;

const char mappa1[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10] = {
  { "S1", "MA1", "MA2", "MA3", "MA8", "S6" },
  { "S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6" },
  { "S7", "MA13", "MA12", "MA11", "MA10", "MA9", "S3" },
  { "S4", "MA14", "MA15", "MA16", "MA12", "S8" },
  { },
};

const char mappa2[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10] = {
  { "S2", "MA5", "MA6", "MA7", "MA3", "MA8", "S6" },
  { "S3", "MA9", "MA10", "MA11", "MA12", "S8" },
  { "S4", "MA14", "MA15", "MA16", "MA12", "S8" },
  { "S6", "MA8", "MA3", "MA2", "MA1", "S1" },
  { "S5", "MA4", "MA3", "MA2", "MA1", "S1" },
};

void getCammino(char* cammino[], int treno){

  // return (char**)malloc(2 * sizeof(char *));
  // return mappa1[];

  for (size_t i = 0; i < MAX_LUNGHEZZA_CAMMINO; i++) {

    // cammino[i] = mappa1[treno][i];

  }

}

void stampaMappa(const char mappa[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10]){

  for (size_t i = 0; i < NUMERO_TRENI; i++) {

    for (size_t j = 0; j < MAX_LUNGHEZZA_CAMMINO; j++) {

      printf("%s ", mappa[i][j]);

    }

    printf("\n");

  }

}
