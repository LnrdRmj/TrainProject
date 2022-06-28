#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include "socketHelper.h"

#define NUMERO_TRENI 5
#define MAX_LUNGHEZZA_CAMMINO 100
#define SERVER_NAME "serveRegistro"

void getCammino(char *, int, char *);
void gestisciRBC(int, char[1024]);
void sendMappaIntera(int, char);
void gestisciTreno(int, char[1024]);

char mappa1[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO] = {
    {"S1;MA1;MA2;MA3;MA8;S6"},
    {"S2;MA5;MA6;MA7;MA3;MA8;S6"},
    {"S7;MA13;MA12;MA11;MA10;MA9;S3"},
    {"S4;MA14;MA15;MA16;MA12;S8"},
    {""},
};

char mappa2[NUMERO_TRENI][MAX_LUNGHEZZA_CAMMINO] = {
    {"S2;MA5;MA6;MA7;MA3;MA8;S6"},
    {"S3;MA9;MA10;MA11;MA12;S8"},
    {"S4;MA14;MA15;MA16;MA12;S8"},
    {"S6;MA8;MA3;MA2;MA1;S1"},
    {"S5;MA4;MA3;MA2;MA1;S1"},
};

int main(){

    int serverFd = createServer(SERVER_NAME, NUMERO_TRENI);

    while (1){

        // I vari client che si connettono fanno una e una sola richiesta al serverRBC
        int clientFd = accettaRichiesta(serverFd);

        char buffer[1024];
        read(clientFd, buffer, 1024);

        // Il server puo' ricevere due tipi di messaggi: uno dal serverRBC per ricevere un'intera
        // mappa (il messaggio sara quindi del tipo AX dove X puo' essere 1 o 2, che significano
        // MAPPA1 o MAPPA2) o una richiesta da un treno per richiedere il proprio cammino in base
        // una mappa. Quindi il primo carattere del messaggio identifica che tipo di richiesta e'
        if (buffer[0] == 'A')
            gestisciRBC(clientFd, buffer);
        else
            gestisciTreno(clientFd, buffer);

        close(clientFd);

    }

    close(serverFd);

    return 0;
}

void gestisciRBC(int clientFd, char buffer[1024]){

    sendMappaIntera(clientFd, buffer[1]);

}

// Costruisce e manda la mappa intera al serverRBC che l'ha richiesta
// Per costruire la mappa intera ho concatenato tutti gli elementi di MAPPA1 o MAPPA2
// in una stringa delimitati dal carattere '/'
void sendMappaIntera(int clientFd, char numeroMappa){

    char *mappaIntera = malloc(256);
    char *tmp = mappaIntera;
    // tmp mi serve perche' viene modificao al posto di mappaIntera

    for (int i = 0; i < NUMERO_TRENI; ++i){

        if (numeroMappa == '1'){

            strcpy(tmp, mappa1[i]);
            tmp += strlen(mappa1[i]);
            *tmp = '/';
            tmp++;
        }
        else if (numeroMappa == '2'){

            strcpy(tmp, mappa2[i]);
            tmp += strlen(mappa2[i]);
            *tmp = '/';
            tmp++;
        }

    }

    send(clientFd, mappaIntera, 256, 0);
}

// Manda semplicemente al richiedente il cammino di un treno
void gestisciTreno(int clientFd, char buffer[1024]){

    int numeroTreno = atoi(&buffer[0]);

    char *mappa = malloc(7);
    memcpy(mappa, &buffer[1], 6);
    mappa[6] = '\0';

    char *cammino = malloc(MAX_LUNGHEZZA_CAMMINO);
    getCammino(cammino, numeroTreno, mappa);
    printf("cammino %s\n", cammino);
    write(clientFd, cammino, strlen(cammino) + 1);

    close(clientFd);

}

// Prende il cammino 
void getCammino(char *cammino, int treno, char *mappa)
{

    if (strcmp(mappa, "MAPPA1") == 0)
        strcpy(cammino, mappa1[treno]);

    else if (strcmp(mappa, "MAPPA2") == 0)
        strcpy(cammino, mappa2[treno]);

}
