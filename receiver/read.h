#include <string.h>

//int read(char *portname, volatile int *running, bool verbose);
int read(char *portname, char *dstname, bool forward, volatile int *running, bool verbose, bool output);