#include <stdio.h>
#include "segmentiManager.h"

#define PREFISSO "MA"

void creaFileSegmento(int);

void creaFileSegmento(int numSegmento){

  char *pathName;
  asprintf(&pathName, "segmenti/%s%d", PREFISSO, numSegmento);
  // int file = open(buf, O_CREAT|O_RDWR, 0666);

  FILE *file = fopen(pathName, "w+");

  // Inizalizzazione a 0 dei file
  fwrite("0", 1, 1, file);

  fclose(file);
  // Setta i permessi corretti del file
  chmod(pathName, 0666);

}
