int startListen(volatile int *running, char * portname, bool verbose, bool ftdidevice);
bool checkPort();
bool start(volatile int *running, char * portname, bool verbose);
bool startftdi(volatile int *running, bool verbose);
