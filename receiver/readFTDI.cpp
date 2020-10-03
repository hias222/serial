/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number

	To build use the following gcc statement 
	(assuming you have the static d2xx library in the /usr/local/lib directory
	and you have created a symbolic link to it in the current dir).
	gcc -o static_link main.c -lpthread libftd2xx.a 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ftd2xx.h"

#include "analyseData.h"

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define BUF_SIZE 0x10

#define MAX_DEVICES 5

static void dumpBuffer(unsigned char *buffer, int elements)
{
	int j;
	for (j = 0; j < elements; j++)
	{
		if ((buffer[j] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
		{
			printf("\n");
		}
		printf("0x%02X ", buffer[j]);
		putReadData(buffer[j]);
	}
}

int readftdi(volatile int *running, bool verbose)
{
	unsigned char *pcBufRead;
	DWORD dwBytesRead;
	FILE *fh;
	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;
	int iport;

	iport = 0;

	printf("receiver - using ftdi lib\n");

	ftStatus = FT_Open(iport, &ftHandle);
	if (ftStatus != FT_OK)
	{
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("receiver - FT_Open(%d) failed\n", iport);
		return 1;
	}

	pcBufRead = (unsigned char *)malloc(BUF_SIZE);

	FT_ResetDevice(ftHandle);
	FT_SetBaudRate(ftHandle, 9600);

	ftStatus = FT_SetDataCharacteristics(ftHandle,
										 FT_BITS_8,
										 FT_STOP_BITS_1,
										 FT_PARITY_EVEN);

	/*
				 Parity -must be FT_PARITY_NONE, FT_PARITY_ODD, FT_PARITY_EVEN, FT_PARITY_MARKor FT_PARITY SPACE
	*/

	if (ftStatus != FT_OK)
	{
		printf("receiver - Failure.  FT_SetDataCharacteristics returned %d.\n",
			   (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - set stop bits\n");
	}

	ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE , 0x00, 0x00);

	//FT_FLOW_NONE Must be one of FT_FLOW_NONE, FT_FLOW_RTS_CTS, FT_FLOW_DTR_DSRor FT_FLOW_XON_XOFF.uXonCharacter used to signal Xon.  Only u

	if (ftStatus != FT_OK)
	{
		printf("receiver - Failure.  FT_SetFlowControl FT_FLOW_NONE returned %d.\n",
			   (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - set FT_SetFlowControl FT_FLOW_NONE\n");
	}

	//FT_SetDtr(ftHandle);
	//FT_SetRts(ftHandle);
	FT_ClrDtr(ftHandle);
	FT_ClrRts(ftHandle);
	FT_SetBreakOff(ftHandle);

	// FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0);
	FT_SetTimeouts(ftHandle, 1, 0); // infinite timeouts
	//FT_SetBitMode(ftHandle, 0xFF, 0x01);

	while (*running)
	{
		ftStatus = FT_Read(ftHandle, pcBufRead, BUF_SIZE, &dwBytesRead);

		if (ftStatus != FT_OK)
		{
			printf("Failure.  FT_Read returned %d.\n", (int)ftStatus);
			return 1;
		}

		dumpBuffer(pcBufRead, (int)dwBytesRead);
	}
	free(pcBufRead);
	FT_Close(ftHandle);

	return 0;
}
