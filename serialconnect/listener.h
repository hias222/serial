int startListen(volatile int *running, char *portname,char *dstportname, bool verbose, bool ftdidevice, bool sendode);
bool checkPort();
bool start(volatile int *running, char * portname, bool verbose);
bool startftdi(volatile int *running, bool verbose);
