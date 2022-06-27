#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "segmentiManager.h"

#define NUMERO_SEGMENTI 16
#define NUMERO_PROCESSI_TRENI 5

// Questo file rappresenta il processo padre

void creaFilesSegmenti(void);
void inizializzaProcessiTreni(void);
void startRegistro(void);
void startTreni(char *, char[10]);

int main(int argc, char *argv[]) {

  creaFilesSegmenti();

  if (strcmp(argv[1], "ETCS2") == 0 && strcmp(argv[2], "RBC") == 0) {

    execl("rbc", "rbc", argv[3], NULL);

    // Se devo solo lanciare il server RBC mi fermo
    return 0;

  }

  startRegistro();

  startTreni(argv[1], argv[2]);

  return 0;

}

// Fa partire il processo registro
void startRegistro() {

  if (fork() == 0) execl("registro", "registro", NULL);

}

// Fa partire i processi treni
void startTreni(char *mode, char mappa[10]) {

  // NUMERO_PROCESSI_TRENI
  for (int i = 0; i < NUMERO_PROCESSI_TRENI; i++) {

    if (fork() == 0){

      char *numeroTreno = malloc(10);
      sprintf(numeroTreno, "%i", i);
      
      execl("treno", "treno", numeroTreno, mode, mappa, NULL);

    }

  }

}

// Crea i file per memorizzare gli stati dei segmenti
void creaFilesSegmenti() {

  for (int i = 1; i <= NUMERO_SEGMENTI; i++)
    creaFileSegmento(i); // parte di segmentiManager.c

}
