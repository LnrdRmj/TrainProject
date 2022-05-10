#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  // Il primo argomento è il numero del treno
  char *numTreno = argv[0];

  printf("Sono il treno numero %s\n", numTreno);

  return 0;

}
