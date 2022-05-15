#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[0];

  printf("Sono il treno numero %s\n", numTreno);

  exit(0);
  return 0;

}
