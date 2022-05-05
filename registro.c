#include "stdio.h"
#include "stdlib.h"

#define NUMERO_TRENI 5
#define MAX_LUNGHEZZA_CAMMINO 10

void stampaMappa (const char mappa[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO][10]);
void getCammino(char * [], int);

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

int main() {

  stampaMappa(mappa1);

  char *cammino[10];
  getCammino(cammino, 2);

  for (size_t i = 0; i < MAX_LUNGHEZZA_CAMMINO; i++) {

      printf("%s\n", *(cammino + i));

  }

}

void getCammino(char* cammino[], int treno){

  // return (char**)malloc(2 * sizeof(char *));
  // return mappa1[];

  for (size_t i = 0; i < MAX_LUNGHEZZA_CAMMINO; i++) {

    cammino[i] = mappa1[treno][i];

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
