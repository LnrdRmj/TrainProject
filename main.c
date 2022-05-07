#include "stdio.h"
#include "fcntl.h"
#include <string.h>
#include "segmentiManager.h"

#define NUMERO_TRATTE 16
#define NUMERO_PROCESSI_TRENI 5

// Questo file rappresenta il processo padre

void creaFiles(void);
void inizializzaProcessiTreni(void);

int main() {

  // open("cartella/text.txt", O_CREAT|O_RDWR, 0777);

  // FILE *file = fopen("segmenti/segmenti.txt", "w");
  // fclose(file);

  creaFiles();

}

void creaFiles() {

  for (int i = 0; i < NUMERO_TRATTE; i++) {

    // char *pathName;
    // asprintf(&pathName, "segmenti/MA%d", i);
    // // int file = open(buf, O_CREAT|O_RDWR, 0666);
    //
    // FILE *file = fopen(pathName, "w+");
    //
    // // Inizalizzazione a 0 dei file
    // fwrite("0", 1, 1, file);
    //
    // fclose(file);
    // chmod(pathName, 0666);

    creaFileSegmento(i);

  }

}
