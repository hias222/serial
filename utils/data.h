
int dataStart();
int dataInit(volatile int *running, char *portname,bool sendmode, bool verbose);
int terminalInit(volatile int *running, char *portname);
int dataClean();