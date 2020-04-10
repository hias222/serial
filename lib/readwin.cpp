#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

// https://gitlab.com/Teuniz/RS-232/-/tree/master

#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#endif

#include "serialData.h"

#define RS232_PORTNR 32
#define COLORADO_ADDRESS_WORD_MASK 0x80

#ifdef _WIN32

bool read(string portstring)
{
    int port = atoi(portstring);
    HANDLE Cport[RS232_PORTNR];
    const char *comports[RS232_PORTNR] = {"\\\\.\\COM1", "\\\\.\\COM2", "\\\\.\\COM3", "\\\\.\\COM4",
                                          "\\\\.\\COM5", "\\\\.\\COM6", "\\\\.\\COM7", "\\\\.\\COM8",
                                          "\\\\.\\COM9", "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                          "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
                                          "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
                                          "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
                                          "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
                                          "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};

    char mode_str[128];
    int comport_number = 2;
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
        hComm = CreateFileA(comports[comport_number],
                            GENERIC_READ | GENERIC_WRITE,
                            0,    /* no share  */
                            NULL, /* no security */
                            OPEN_EXISTING,
                            0,     /* no threads */
                            NULL); /* no templates */

        if (hComm == INVALID_HANDLE_VALUE)
        {
            printf_s("\n Port can't be opened\n\n");
            return false;
        }

        //Setting the Parameters for the SerialPort
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

        Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
        if (Status == FALSE)
        {
            printf_s("\nError to Get the Com state\n\n");
            return false;
        }

        dcbSerialParams.BaudRate = CBR_9600;   //BaudRate = 9600
        dcbSerialParams.ByteSize = 8;          //ByteSize = 8
        dcbSerialParams.StopBits = ONESTOPBIT; //StopBits = 1
        dcbSerialParams.Parity = NOPARITY;     //Parity = None

        Status = SetCommState(hComm, &dcbSerialParams);

        if (Status == FALSE)
        {
            printf_s("\nError to Setting DCB Structure\n\n");
            return false;
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
            return false;
        }

        //Setting Receive Mask
        Status = SetCommMask(hComm, EV_RXCHAR);
        if (Status == FALSE)
        {
            printf_s("\nError to in Setting CommMask\n\n");
            return false;
        }

        /*
        //Setting WaitComm() Event
        Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

        if (Status == FALSE)
        {
            printf_s("\nError! in Setting WaitCommEvent()\n\n");
            return false;
        }
        */

        //Read data and store in a buffer

        // Create an event object for use by WaitCommEvent.

        o.hEvent = CreateEvent(
            NULL,  // default security attributes
            TRUE,  // manual-reset event
            FALSE, // not signaled
            NULL   // no name
        );

        serialData myReaddata = serialData();

        // Initialize the rest of the OVERLAPPED structure to zero.
        o.Internal = 0;
        o.InternalHigh = 0;
        o.Offset = 0;
        o.OffsetHigh = 0;

        assert(o.hEvent);

        bool connectsuccess = true;

        do 
        {
        if (WaitCommEvent(hComm, &dwEventMask, &o))
        {
            
            if (dwEventMask & EV_RXCHAR)
            {
                do
                {
                    Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
                    myReaddata.putData(ReadData);
                    ++loop;
                } while (NoBytesRead > 0);
                --loop; //Get Actual length of received data
                printf_s("\nNumber of bytes received = %d\n\n", loop);
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
        printf("wait ... \n");
        } while (connectsuccess);

        CloseHandle(hComm); //Closing the Serial Port
        printf("closing \n");
    }
    catch (...)
    {
        printf("Exception occurred");
        return false;
    }
    printf("end \n");
    return true;
}

#endif