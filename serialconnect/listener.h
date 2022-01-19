int startListen(volatile int *running, char * portname, bool verbose, bool ftdidevice, bool sendmode);
bool checkPort();
bool start(volatile int *running, char * portname, bool verbose);
bool startftdi(volatile int *running, bool verbose);
