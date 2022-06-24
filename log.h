FILE* creaFileLogTreno(int);
void logStatoTreno(char*, char*, FILE*);
void logInizioViaggio(char*, FILE*);
void logFineViaggio(char*, FILE*);
void logTrenoBloccato(char*, FILE*);
FILE* creaFileLogServerRBC();
void logRBCStazioneConcessa(char*, FILE*);
void logRBCSegmentoConcesso(int, FILE*);
void logRBCSegmentoNegato(int, FILE*);