// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

// https://gitlab.com/Teuniz/RS-232/-/tree/master

#include <Windows.h>

#define RS232_PORTNR 32

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

// https://aticleworld.com/serial-port-programming-using-win32-api/

bool open(int port)
{
    int comport_number = 2;
    BOOL Status;                 // Status
    DCB dcbSerialParams = {0};   // Initializing DCB structure
    char SerialBuffer[64] = {0}; //Buffer to send and receive data
    DWORD BytesWritten = 0;      // No of bytes written to the port


    printf("try to connect to port %s\n",comports[comport_number] );
    printf("please check that the port gives an answer \n" );

    Cport[comport_number] = CreateFileA(comports[comport_number],
                                        GENERIC_READ | GENERIC_WRITE,
                                        0,    /* no share  */
                                        NULL, /* no security */
                                        OPEN_EXISTING,
                                        0,     /* no threads */
                                        NULL); /* no templates */

    if (Cport[comport_number] == INVALID_HANDLE_VALUE)
    {
        printf("unable to open comport\n");
        return false;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(Cport[comport_number], &dcbSerialParams); //retreives  the current settings

    if (Status == FALSE)
    {
        printf_s("\nError to Get the Com state\n\n");
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;   //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;          //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT; //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;     //Parity = None

    Status = SetCommState(Cport[comport_number], &dcbSerialParams);
    if (Status == FALSE)
    {
        printf_s("\nError to Setting DCB Structure\n\n");
        return false;
    }

    sprintf(SerialBuffer, "Hello");

    Status = WriteFile(Cport[comport_number], // Handle to the Serialport
                       SerialBuffer,          // Data to be written to the port
                       sizeof(SerialBuffer),  // No of bytes to write into the port
                       &BytesWritten,         // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        printf_s("\nFail to Written");
        return false;
    }

     CloseHandle(Cport[comport_number]); //Closing the Serial Portvvv
    return true;
}