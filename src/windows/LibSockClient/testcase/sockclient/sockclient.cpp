// (C) Copyright 2008 - 2009 by CCTEC ENGINEERING CO., LTD
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//

//-----------------------------------------------------------------------------
//----- $Id$
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "LibSockClient.h"

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024	// for 1KB for test purpose only.

int main(int argc, char* argv[])
{
	bool err;
	char pBuffer[BUFF_SIZE];
	int i;

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

	for (i = 0; i < 10; i++) 
	{
		printf("sending ...\n");
		libsockclient_send("sirtoozee", true);
		printf("receiving ...\n");
		memset(pBuffer, 0, BUFF_SIZE);
		libsockclient_recv(pBuffer, BUFF_SIZE, true);
		printf("%s\n", pBuffer);
	}

	libsockclient_cleanup();
	return 0;
}
