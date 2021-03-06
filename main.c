#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "segmentiManager.h"

#define NUMERO_TRATTE 16
#define NUMERO_PROCESSI_TRENI 5

// Questo file rappresenta il processo padre

void creaFiles(void);
void inizializzaProcessiTreni(void);
void startRegistro(void);
void startTreni(char *, char[10]);

int main(int argc, char *argv[]) {

  // TODO: controllare gli argomenti

  creaFiles();

  if (strcmp(argv[1], "ETCS2") == 0 && strcmp(argv[2], "RBC") == 0) {

    execl("rbc", "rbc", argv[3], NULL);

    return 0;

  }

  startRegistro();

  startTreni(argv[1], argv[2]);

  return 0;

}

void startRegistro() {
  
  if (fork() == 0) execl("registro", "registro", NULL);

}

void startTreni(char *mode,char mappa[10]) {

  //NUMERO_PROCESSI_TRENI
  for (int i = 0; i < NUMERO_PROCESSI_TRENI; i++) {

    // Fai partire i proessi treni
    int pid = fork();
    if (pid == 0){

      char *numeroTreno;
      asprintf(&numeroTreno, "%i", i);

      execl("treno", "treno", numeroTreno, mode, mappa, NULL);

    }

  }

}

void creaFiles() {

  for (int i = 1; i <= NUMERO_TRATTE; i++) {

    creaFileSegmento(i);

  }

}
