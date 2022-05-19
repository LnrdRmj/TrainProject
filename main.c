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
void startTreni(void);

int main() {

  creaFiles();

  startRegistro();

  startTreni();

  return 0;

}

void startRegistro() {

  int pid = fork();
  if (pid == 0) {

    execl("registro", NULL);

  }

}

void startTreni() {

  for (int i = 0; i < NUMERO_PROCESSI_TRENI; i++) {

    // Fai partire i proessi treni
    int pid = fork();
    if (pid == 0){

      char *buffer;
      asprintf(&buffer, "%i", i);

      execl("treno", buffer, NULL);

    }

  }

}

void creaFiles() {

  for (int i = 0; i < NUMERO_TRATTE; i++) {

    creaFileSegmento(i);

  }

}
