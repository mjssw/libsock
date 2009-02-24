// sockclient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LibSockClient.h"

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024	// for 1KB for test purpose only.

int main(int argc, char* argv[])
{
	bool err;
	char pBuffer[BUFF_SIZE];

	err = libsockclient_init("192.168.3.83", 8080);
	if (err) 
	{
		printf("Initial libsockclient success.\n");
	} 
	else 
	{
		printf("fail to initial libsockclient error code %d.\n", 
			libsockclient_geterrorcode());
		return -1;
	}

	printf("sending ...\n");
	libsockclient_send("test", true);
	printf("reciving ...\n");
	memset(pBuffer, 0, BUFF_SIZE);
	libsockclient_recv(pBuffer, true);
	printf("%s\n", pBuffer);

	libsockclient_cleanup();
	return 0;
}
