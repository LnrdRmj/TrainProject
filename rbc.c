#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>

#include "segmentiManager.h"
#include "socketHelper.h"
#include "log.h"

#define SERVER_RBC_NAME "serverRBC"
#define SERVER_QUEUE_LENGTH 10
#define NUMERO_TRATTE 16
#define NUMERO_TRENI 5
#define NUMERO_STAZIONI 8
#define SERVER_REGISTRO "serveRegistro"
#define DEBUG true

void setup(char*);
void cicloDiVita(struct pollfd*, int, int);
void gestisciRichiesta(int, char*);
bool isRichiestaOccupazione(char *);
void gestisciOccupazione(int, char*);
bool isRichiestaVerificaSegmentoLibero(char *);
void gestisciVerificaSegmentoLibero(int, char *);
bool isRichiestaStazione(char *);
void gestisciStazione(int, char*);
bool isRichiestaRilascio(char *);
void gestisciRilascio(int, char *);
char* getPercorsi(int, char *);

bool segmenti[NUMERO_TRATTE];
int stazioni[NUMERO_STAZIONI];
FILE* logFile;
char* mappa;

int main(int argc, char const *argv[])
{
	
	printf("partito il server RBC\n");

	// nomeMappa puo' essere MAPPA1 o MAPPA2
	char* nomeMappa = argv[1];

	setup(nomeMappa);

	// Inizializzazione struttre dati per il serverRBC
	int serverFd = createServer(SERVER_RBC_NAME, SERVER_QUEUE_LENGTH);

	// Numero di file massimo che il server gestisce
	// In realta' non sono file veri e propri ma socket AF_UNIX
	int nfds = 10;

	struct pollfd *fds = calloc(nfds, sizeof(struct pollfd));

	// Inizializzo il primo elemento dell'array con le informazioni del server
	// POLLIN mi permette di essere 'notificato' quando il server avra' qualcosa
	// da leggere, cioe' una richiesta di connessione da parte di un processo
	// treno
	fds[0].fd = serverFd;
	fds[0].events = POLLIN;

	for (int i = 1; i < nfds; ++i)
	{
		// Ponendo .fd a -1 faccio in modo che nella funzione poll questo FileDescriptor
		// sia completamente ignorato, migliorandone l'efficienza 
		fds[i].fd = -1;
		fds[i].events = POLLIN;

	}

	cicloDiVita(fds, nfds, serverFd);

	close(serverFd);

	return 0;

}

void setup(char* nomeMappa) {
 
	// Inizializzo lo stato dei segmenti
	for (int i = 0; i < NUMERO_TRATTE; ++i){
		segmenti[i] = false;
	}

	// Inizializzo lo stato delle stazioni
	for (int i = 0; i < NUMERO_STAZIONI; ++i){
		stazioni[i] = 0;
	}

	logFile = creaFileLogServerRBC();

	int registroFd = creaConnessioneAServer(SERVER_REGISTRO);
	mappa = getPercorsi(registroFd, nomeMappa);
	printf("mappa totale %s\n", mappa);

}

char* getPercorsi(int registroFd, char * mappa){

	char *messaggio = malloc(256);

	// Il messaggio puo' essere A1 o A2 (sarebbe All1, per richiedere tutta la mappa1 o 
	// All2 per richiedere tutta la mappa2). Per conoscere il numero della mappa mi basta prendere
	// il sesto carattere della stringa MAPPAX.
	sprintf(messaggio, "A%c", mappa[5]);

	send(registroFd, messaggio, 10, 0);

	recv(registroFd, messaggio, 256, 0);

	return messaggio;

}

void cicloDiVita(struct pollfd* fds, int nfds, int serverFd) {

	int clientConnessi = 0;

	while(1) {

		poll(fds, nfds, -1);

		for (int i = 0; i < nfds; ++i)
		{
			
			// revents mi indica se un determinato fildescriptor ha effettivamente
			// qualcosa da leggere
			if (fds[i].revents == 0) continue;

			// Se il 'file' che e' pronto a essere letto e' il serverFd
			// allora vuol dire che c'e' una richiesta di connessione
			if (fds[i].fd == serverFd){

				// Nuova connessione
				fds[1 + clientConnessi].fd = accettaRichiesta(serverFd);
				fds[1 + clientConnessi++].events = POLLIN;
				if(DEBUG)
					printf("Connesso un nuovo client\n");

			}
			// Altrimenti si tratta di una richiesta di connessione di un processo treno
			else {

				char* buffer = malloc(10);
				int dimensione = recv(fds[i].fd, buffer, 10, 0);

				// Se il il client ha chiuso il socket (o per altro, tipo erorri)
				if (dimensione == 0) {
					close(fds[i].fd);
					fds[i].fd = -1;
					continue;
				}

				gestisciRichiesta(fds[i].fd, buffer);

			}

		}

	}

}

// Divide le varia richieste ad un "header", che in realta' e' semplicemente
// la prima lettera del messaggio
void gestisciRichiesta(int clientFd, char *messaggio) {

	if(DEBUG)
		printf("letto: %s\n", messaggio);

    if (isRichiestaOccupazione(messaggio) == true){

    	gestisciOccupazione(clientFd, messaggio);

    }
    else if (isRichiestaVerificaSegmentoLibero(messaggio) == true){

    	gestisciVerificaSegmentoLibero(clientFd, messaggio);

    }
    else if(isRichiestaStazione(messaggio) == true){

    	gestisciStazione(clientFd, messaggio);

    }
    else if(isRichiestaRilascio(messaggio) == true) {

		gestisciRilascio(clientFd, messaggio);

    }

}

//-------------------------------------------------------------
// Il messaggio per occupare un segmento
// e' del tipo OX dove X sara' un numero che identifica un qualche segmento
bool isRichiestaOccupazione(char *richiesta) {

	return *richiesta == 'O';

}

void gestisciOccupazione(int clientFd, char* messaggio) {

	messaggio++;
	segmenti[strtol(messaggio, NULL, 10) - 1] = true;

}

//-------------------------------------------------------------
// Il messaggio per verificare se un segmento e' libero oppure no
// e' del tipo LX dove X sara' un numero che identifica un qualche segmento
bool isRichiestaVerificaSegmentoLibero(char *richiesta) {

	return *richiesta == 'L';

}

void gestisciVerificaSegmentoLibero(int clientFd, char *messaggio) {

	// Salto la prima lettera
	messaggio++;

	int numeroSegmento = strtol(messaggio, NULL, 10);
	if (segmenti[numeroSegmento - 1] == false){
		logRBCSegmentoConcesso(numeroSegmento, logFile);
		*messaggio = '1';
	}
	else {
		logRBCSegmentoNegato(numeroSegmento, logFile);
		*messaggio = '0';
	}

	send(clientFd, messaggio, 10, 0);

}

//-------------------------------------------------------------
// Il messaggio per verificare se un segmento e' libero oppure no
// e' del tipo SX dove X sara' un numero che identifica la stazione che
// processo treno vuole accedere
bool isRichiestaStazione(char *richiesta) {

	return *richiesta == 'S';

}

void gestisciStazione(int clientFd, char* messaggio) {

	if(DEBUG)
		printf("stazione concessa: %s\n", messaggio);

	send(clientFd, "1", 10, 0);
	logRBCStazioneConcessa(messaggio, logFile);

}

//-------------------------------------------------------------
// Il messaggio per liberare un segmento
// e' del tipo RX, dove X sara' un numero che identifica un qualche segmento
bool isRichiestaRilascio(char *richiesta) {

	return *richiesta == 'R';

}

void gestisciRilascio (int clientFd, char* messaggio){

	// Salto la prima lettera
	messaggio++;
	//libero il segmento
	segmenti[strtol(messaggio, NULL, 10) - 1] = false;

	if(DEBUG)
		printf("segmento liberato %lu\n", strtol(messaggio, NULL, 10));

}