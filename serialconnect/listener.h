int startListen(volatile int *running, char *portname,char *dstportname, bool verbose, bool ftdidevice, bool sendode, bool outputdata);
bool checkPort();
bool start(volatile int *running, char * portname, bool verbose, bool output);
bool startftdi(volatile int *running, bool verbose);
