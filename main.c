#include "stdio.h"
#include "fcntl.h"
#include <string.h>

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

  printf("Sto creando i file\n");
  for (int i = 0; i < NUMERO_TRATTE; i++) {

    char *pathName;
    asprintf(&pathName, "segmenti/MA%d", i);
    // printf("%s\n", buf);
    // int file = open(buf, O_CREAT|O_RDWR, 0666);

    FILE *file = fopen(pathName, "w+");

    fwrite("0", 1, 1, file);

    fclose(file);
    chmod(pathName, 0666);

  }

}
