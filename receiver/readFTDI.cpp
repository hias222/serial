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
#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <time.h>
#else
#include <unistd.h>
#endif
#include "ftd2xx.h"

#include "analyseData.h"

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define BUF_SIZE 0x10

#define MAX_DEVICES 5

static volatile int keepRunning;

static void dumpBuffer(unsigned char *buffer, int elements, bool verbose)
{
	int j;
	for (j = 0; j < elements; j++)
	{
		if (verbose)
		{
			if ((buffer[j] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
			{
				printf("\n");
			}
			printf("0x%02X ", buffer[j]);
		}
		putReadData(buffer[j]);
	}
}

static void dumpToFile(unsigned char *buffer, int elements, FILE *filename)
{
	int j;
	for (j = 0; j < elements; j++)
	{
		fprintf(filename,"%02X", buffer[j]);
	}
}

DWORD getNumberOfFtdiDevices()
{
	FT_STATUS ftStatus;
	FT_DEVICE_LIST_INFO_NODE *devInfo;
	DWORD numDevs;
	// create the device information list
	ftStatus = FT_CreateDeviceInfoList(&numDevs);
	if (ftStatus == FT_OK)
	{
		printf("Number of devices is %d\n", numDevs);
	}
	return numDevs;
}

int openFtdiHanlde(int numberDevice, FT_HANDLE *prepareHandle)
{
	FT_STATUS ftStatus;

	ftStatus = FT_Open(numberDevice, prepareHandle);

	if (ftStatus != FT_OK)
	{
		printf("receiver - %d - FT_Open (Error: %d) failed\n", numberDevice, (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - %d - FT_Open success\n", numberDevice);
	}

	ftStatus = FT_ResetDevice(*prepareHandle);
	if (ftStatus != FT_OK)
	{
		printf("receiver - %d - Failure.  FT_ResetDevice returned %d.\n",
			   numberDevice, (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - %d - FT_ResetDevice\n", numberDevice);
	}

	ftStatus = FT_SetBaudRate(*prepareHandle, 9600);

	if (ftStatus != FT_OK)
	{
		printf("receiver - %d - Failure.  FT_SetBaudRate returned %d.\n",
			   numberDevice, (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - %d - set Baud Rate bits\n", numberDevice);
	}

	ftStatus = FT_SetDataCharacteristics(*prepareHandle,
										 FT_BITS_8,
										 FT_STOP_BITS_1,
										 FT_PARITY_EVEN);

	// Parity -must be FT_PARITY_NONE, FT_PARITY_ODD, FT_PARITY_EVEN, FT_PARITY_MARKor FT_PARITY SPACE

	if (ftStatus != FT_OK)
	{
		printf("receiver - %d - Failure.  FT_SetDataCharacteristics returned %d.\n",
			   numberDevice, (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - %d - set stop bits\n", numberDevice);
	}

	ftStatus = FT_SetFlowControl(*prepareHandle, FT_FLOW_NONE, 0x00, 0x00);

	// FT_FLOW_NONE Must be one of FT_FLOW_NONE, FT_FLOW_RTS_CTS, FT_FLOW_DTR_DSRor FT_FLOW_XON_XOFF.uXonCharacter used to signal Xon.  Only u

	if (ftStatus != FT_OK)
	{
		printf("receiver - %d - Failure.  FT_SetFlowControl FT_FLOW_NONE returned %d.\n",
			   numberDevice, (int)ftStatus);
		return 1;
	}
	else
	{
		printf("receiver - %d - set FT_SetFlowControl FT_FLOW_NONE\n", numberDevice);
	}

	// FT_SetDtr(ftHandle);
	// FT_SetRts(ftHandle);
	FT_ClrDtr(*prepareHandle);
	FT_ClrRts(*prepareHandle);
	FT_SetBreakOff(*prepareHandle);

	// FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0, 0);
	FT_SetTimeouts(*prepareHandle, 1, 0); // infinite timeouts
										  // FT_SetBitMode(ftHandle, 0xFF, 0x01);

	return 0;
}

int readftdi(volatile int *running, bool verbose, bool outputdata)
{
	unsigned char *pcBufRead;
	DWORD dwBytesRead, dwBytesWrite;
	FILE *fh;
	FT_HANDLE ftHandle;
	FT_HANDLE sendftHandle;
	FT_STATUS ftStatus;
	FILE *outputfile;

	FT_DEVICE_LIST_INFO_NODE *devInfo;

	bool sendmode = false;

	int iport;

	pcBufRead = (unsigned char *)malloc(BUF_SIZE);

	iport = 0;

	printf("receiver - using ftdi lib\n");

#ifdef _WIN32
	printf("receiver - windows ok\n");
#else
	uid_t uid = getuid(), euid = geteuid();
	if (uid < 1 || uid != euid)
	{
		printf("receiver - root privs ok\n");
		printf("receiver - check sudo rmmod ftdi_sio \n");
		printf("receiver - check configured lanes on scoreboard \n");
		printf("receiver - add ftdi_sio to /etc/modprobe.d/blacklist.conf \n");
	}
	else
	{
		printf("receiver - %d %d - for usb access we need root privs\n", uid, euid);
		keepRunning = 0;
		running = &keepRunning;
		// return 1;
	}
#endif

	DWORD numFtdisvs = getNumberOfFtdiDevices();
	printf("found %d ftdi devices\n", numFtdisvs);

	if (numFtdisvs > 1)
	{
		sendmode = true;
	}

	devInfo =
		(FT_DEVICE_LIST_INFO_NODE *)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) * numFtdisvs);

	for (int i = 0; i < numFtdisvs; i++)
	{
		openFtdiHanlde(i, &devInfo[i].ftHandle);
	}

	if (outputdata)
	{
		// log to file
		outputfile = fopen("rawdata.txt", "w+");
	}

	int checkloop = 0;
	while (*running)
	{
		checkloop++;
		ftStatus = FT_Read(devInfo[0].ftHandle, pcBufRead, BUF_SIZE, &dwBytesRead);

		if (ftStatus != FT_OK)
		{
			printf("Failure.  FT_Read returned %d.\n", (int)ftStatus);
			return 1;
		}

		if (outputdata)
		{
			// log to file
			dumpToFile(pcBufRead, (int)dwBytesRead, outputfile);
		}

		if (sendmode)
		{

			ftStatus = FT_Write(devInfo[1].ftHandle, pcBufRead, BUF_SIZE, &dwBytesWrite);

			if (ftStatus != FT_OK)
			{
				printf("Failure.  FT_Write returned %d.\n", (int)ftStatus);
				return 1;
			}
		}

		dumpBuffer(pcBufRead, (int)dwBytesRead, verbose);
	}
	free(pcBufRead);

	if (outputdata)
	{
		fclose(outputfile);
	}

	FT_Close(devInfo[0].ftHandle);
	FT_Close(devInfo[1].ftHandle);

	free(devInfo);

	return 0;
}
