#include <stdio.h>
#include <string.h>
#include <string>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

// https://gitlab.com/Teuniz/RS-232/-/tree/master

#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#endif

#include "analyseData.h"

#define RS232_PORTNR 32
#define COLORADO_ADDRESS_WORD_MASK 0x80

//#define debug_read

using namespace std;

int read(char *portname, volatile int *running, bool verbose)
{
    //printf("port %s\n", portname);
    char subbuff[2];
    memcpy(subbuff, &portname[3], 2);
    int portnumber = atoi(subbuff);
    int comport_number = portnumber - 1;
    //HANDLE Cport[RS232_PORTNR];
    const char *comports[RS232_PORTNR] = {"\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4",
                                          "\\\\.\\COM5", "\\\\.\\COM6", "\\\\.\\COM7", "\\\\.\\COM8",
                                          "\\\\.\\COM9", "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                          "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
                                          "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
                                          "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
                                          "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
                                          "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};

    //char mode_str[128];
    HANDLE hComm; // Handle to the Serial port
    BOOL Status;  // Status
    OVERLAPPED o;
    DCB dcbSerialParams = {0};   // Initializing DCB structure
    COMMTIMEOUTS timeouts = {0}; //Initializing timeouts structure
    DWORD BytesWritten = 0;      // No of bytes written to the port
    DWORD dwEventMask;           // Event mask to trigger
    //char ReadData;               //temperory Character
    uint8_t ReadData;
    DWORD NoBytesRead; // Bytes read by ReadFile()
    unsigned char loop = 0;
    wchar_t pszPortName[10] = {0}; //com port id
    wchar_t PortNo[20] = {0};      //contain friendly name
    try
    {
        printf("Using serial port = %s\n", comports[comport_number]);

        hComm = CreateFileA(comports[comport_number],
                            GENERIC_READ | GENERIC_WRITE,
                            0,    /* no share  */
                            NULL, /* no security */
                            OPEN_EXISTING,
                            0,     /* no threads */
                            NULL); /* no templates */

        if (hComm == INVALID_HANDLE_VALUE)
        {
            printf_s("Port can't be opened\n");
            printf_s("waiting ...\n");
            Sleep(5000);
            printf_s("try again\n\n");
            return 1;
        }

        //Setting the Parameters for the SerialPort
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
        if (Status == FALSE)
        {
            printf_s("\nError to Get the Com state\n\n");
            return 1;
        }

        dcbSerialParams.BaudRate = CBR_9600;   //BaudRate = 9600
        dcbSerialParams.ByteSize = 8;          //ByteSize = 8
        dcbSerialParams.StopBits = ONESTOPBIT; //StopBits = 1
        dcbSerialParams.Parity = NOPARITY;     //Parity = None

        Status = SetCommState(hComm, &dcbSerialParams);

        if (Status == FALSE)
        {
            printf_s("\nError to Setting DCB Structure\n\n");
            return 1;
        }

        //Setting Timeouts
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;

        if (SetCommTimeouts(hComm, &timeouts) == FALSE)
        {
            printf_s("\nError to Setting Time outs");
            return 1;
        }

        //Setting Receive Mask
        Status = SetCommMask(hComm, EV_RXCHAR);
        if (Status == FALSE)
        {
            printf_s("\nError to in Setting CommMask\n\n");
            return 1;
        }

        //Read data and store in a buffer
        // Create an event object for use by WaitCommEvent.

        o.hEvent = CreateEvent(
            NULL,  // default security attributes
            TRUE,  // manual-reset event
            FALSE, // not signaled
            NULL   // no name
        );

        // Initialize the rest of the OVERLAPPED structure to zero.
        o.Internal = 0;
        o.InternalHigh = 0;
        o.Offset = 0;
        o.OffsetHigh = 0;

        assert(o.hEvent);
        bool connectsuccess = true;
        printf("  Serial port = %s\n", comports[comport_number]);

        do
        {
            if (WaitCommEvent(hComm, &dwEventMask, &o))
            {

                if (dwEventMask & EV_RXCHAR)
                {
                    do
                    {
                        Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);

                        if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
                        {
                            if (verbose)
                            {
                                printf("\n");
                            }
#ifdef debug_read
                            printf("\n %02x \n", COLORADO_ADDRESS_WORD_MASK);
#endif
                            order = 0;
                        }

                        if (verbose)
                        {
                            printf("%02x ", ReadData);
                        }
                        putReadData(ReadData);
                        ++loop;
                    } while (NoBytesRead > 0);
                    --loop; //Get Actual length of received data
#ifdef debug_read
                    printf_s("\nNumber of bytes received = %d\n\n", loop);
#endif
                }
                else
                {
                    printf("start with %d \n", dwEventMask);
                }
            }
            else
            {
                connectsuccess = false;
                DWORD dwRet = GetLastError();
                if (ERROR_IO_PENDING == dwRet)
                {
                    printf("I/O is pending...\n");

                    // To do.
                }
                else
                    printf("Wait failed with error %d.\n", GetLastError());
            }
#ifdef debug_read
            printf("wait ... \n");
#endif
        } while (*running);

        CloseHandle(hComm); //Closing the Serial Port
        printf("serial closing \n");
    }
    catch (...)
    {
        printf("Exception occurred");
        return 1;
    }
    printf("end \n");
    return 0;
}
