#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>

#include "segmentiManager.h"
#include "socketHelper.h"

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10
#define NUMERO_TRATTE 16
#define NUMERO_TRENI 5
#define NUMERO_STAZIONI 8
#define SERVER_REGISTRO "serveRegistro"

void gestisciRichiesta(int);
// void gestisciRilascio (int, char*);
void gestisciOccupazione(int, char*);
void gestisciVerificaSegmentoLibero(int, char *);
bool isRichiestaOccupazione(char *);
bool isRichiestaVerificaSegmentoLibero(char *);
bool isRichiestaStazione(char *);
char* getPercorsi(int, char *);
void setup();

bool segmenti[NUMERO_TRATTE];
int stazioni[NUMERO_STAZIONI];

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

	setup();

	char *mappa = argv[1];
	
	// int registroFd = creaConnessioneAServer(SERVER_REGISTRO);
	
	// char *percorsi = getPercorsi(registroFd, mappa);

	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);
	// ioctl(serverFd, );

	int clientConnessi = 0;
	int nfds = 3;
	struct pollfd *fds = malloc(nfds * sizeof(struct pollfd));
	
	fds[0].fd = serverFd;
	fds[0].events = POLLIN;

	for (int i = 1; i < nfds; ++i)
	{

		fds[i].fd = -1;
		fds[i].events = POLLIN;

	}

	while(1) {

		printf("Aspetto connessioni o richeista dai treni\n");
		poll(fds, nfds, -1);

		for (int i = 0; i < nfds; ++i)
		{
			
			if (fds[i].revents == 0) continue;

			if (fds[i].fd == serverFd){

				// Nuova connessione
				fds[1 + clientConnessi].fd = accettaRichiesta(serverFd);
				printf("creato il socket con fd: %i\n", fds[1 + clientConnessi].fd);
				fds[1 + clientConnessi++].events = POLLIN;
				printf("Connesso un nuovo client\n");

			}
			else {

				// Si tratta di un client che fa una richiesta
				printf("il client con fd: %i\n", fds[i].fd);

				char * buffer = malloc(10);
				int size = recv(fds[i].fd, buffer, 20, 0);

				// Se il il client ha chiuso il socket (o per altro, tipo erorri)
				if (size == 0) {
					close(fds[i].fd);
					fds[i].fd = -1;
				}

				printf("ho letto %d byte\n", size);
				printf("qualcosa da client: %s\n", buffer);

			}

		}

		sleep(1);

		// gestisciRichiesta(clientFd);

	}

	close(serverFd);

	return 0;

}

void setup() {

	// Inizializzo lo stato dei segmenti
	for (int i = 0; i < NUMERO_TRATTE; ++i){
		segmenti[i] = false;
	}

	// Inizializzo lo stato delle stazioni
	for (int i = 0; i < NUMERO_STAZIONI; ++i){
		stazioni[i] = 0;
	}

}

char* getPercorsi(int registroFd, char * mappa){


	char * message = malloc(10);

	sprintf(message, "A%c", mappa[5]);
	printf("%s\n", message);

	send(registroFd, message, 10, 0);

	recv(registroFd, message, 1024, 0);
	printf("%s\n", message);

	return message;

}

void gestisciRichiesta(int clientFd) {

	while(1 && clientFd != -1) {

		char *messaggio = malloc(10);
		printf("Aspetto una nuoca richiesta\n");
	    recv(clientFd, messaggio, 10, 0);

	    printf("RBC - Ho letto %s\n", messaggio);

	    if (isRichiestaOccupazione(messaggio) == true){

	    	gestisciOccupazione(clientFd, messaggio);

	    }
	    else if (isRichiestaVerificaSegmentoLibero(messaggio) == true){

	    	// gestisciRilascio(clientFd, messaggio);
	    	gestisciVerificaSegmentoLibero(clientFd, messaggio);


	    }
	    else if(isRichiestaStazione(messaggio) == true){

	    	// Acetta sempre la richiesta

	    }

	}

}

void gestisciRilascio (int clientFd, char* messaggio){

	// Salto la prima lettera
	messaggio++;
	//libero il segmento
	segmenti[strtol(messaggio, NULL, 10)] = false;
	printf("segmento liberato %lu\n", strtol(messaggio, NULL, 10));

}

void gestisciOccupazione(int clientFd, char* messaggio) {

	char *response = malloc(10);

	// Salto la prima lettera
	messaggio++;

	// Se il segmento e' libero allora lo occupo
	if (segmenti[strtol(messaggio, NULL, 10)] == false) {

		printf("segmento %lu preso\n", strtol(messaggio, NULL, 10));
		*response = '1';

	}
	// Altrimenti era gia occupato
	else {

		printf("segmento %lu non preso\n", strtol(messaggio, NULL, 10));
		*response = '0';

	}

	printf("server - Mando %s\n", response);
	send(clientFd, response, 10, 0);

}

void gestisciVerificaSegmentoLibero(int clientFd, char *messaggio) {

	// Salto la prima lettera
	messaggio++;

	if (segmenti[strtol(messaggio, NULL, 10)] == false)
		*messaggio = '1';
	else 
		*messaggio = '0';

	*(messaggio++) = '\0';

	send(clientFd, messaggio, 10, 0);

}

bool isRichiestaOccupazione(char *richiesta) {

	return *richiesta == 'O';

}

bool isRichiestaVerificaSegmentoLibero(char *richiesta) {

	return *richiesta == 'L';

}

bool isRichiestaStazione(char *richiesta) {

	return *richiesta == 'S';

}

// char* getCamminoTreno(int registro, int numeroTreno, char *mappa){

// 	char *buffer;
// 	sprintf(buffer, "%i", numeroTreno); 

// 	char *dataForServer = strcat(buffer, mappa);
// 	write(registro, dataForServer, strlen(dataForServer) + 1);

// 	char *cammino = malloc(100);
// 	read(registro, cammino, 100);

// 	return cammino;

// }