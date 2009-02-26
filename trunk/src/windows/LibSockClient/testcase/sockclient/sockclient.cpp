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
#include <windows.h>

#define BUFF_SIZE 1024	// for 1KB for test purpose only.

static char *m_msg[] = 
{
	"sirtoozee",
	"open source",
	"Linux",
	"Unix",
	"PSP",
	"iphone",
	"XBOX",
	"SDK",
	"C/C++",
	"Java"
};

int main(int argc, char* argv[])
{
	bool err;
	char pBuffer[BUFF_SIZE];
	int i = 0;
	bool loop = true;

	err = libsockclient_init(argv[1] ? argv[1] : "192.168.3.83", 8080);
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

	while (loop) 
	{
		if (i == 13) loop = false;
		Sleep(1000);
		printf("tick %d...\t", i);
		i++;
	}

	for (i = 0; i < 10; i++) 
	{
		Sleep(6000);
		printf("sending %s...\n", m_msg[i]);
		libsockclient_send(m_msg[i], true);
		printf("receiving ...\n");
		memset(pBuffer, 0, BUFF_SIZE);
		libsockclient_recv(pBuffer, BUFF_SIZE, true);
		printf("%s\n", pBuffer);
	}

	libsockclient_cleanup();
	return 0;
}
