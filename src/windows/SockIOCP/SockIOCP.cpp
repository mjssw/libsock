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

#include "XGetopt.h"	// for getopt

#define _WIN32_WINNT 0x0500
#include <Winsock2.h>	// for Winsock2
#include <Winbase.h>	// for BindIoCompletionCallback
#include <stdio.h>		// for printf
#include <conio.h>

//-----------------------------------------------------------------------------
//----- Constant
#define TCP_PORT 8000
#define MAX_CONN_NUM 1000

//-----------------------------------------------------------------------------
//----- Private attribute
static SOCKET m_listener;
static HANDLE m_waiter;

//-----------------------------------------------------------------------------
//----- Private operation
static void m_cleanup();
static void m_IOCallback(DWORD dwErrorCode, 
						 DWORD dwNumberOfBytesTransferred, 
						 LPOVERLAPPED lpOverlapped);

// TODO: To cleanup stuff...
static void 
m_cleanup() 
{
	closesocket(m_listener);
	CloseHandle(m_waiter);
}

static void 
m_IOCallback(DWORD dwErrorCode, 
			 DWORD dwNumberOfBytesTransferred, 
			 LPOVERLAPPED lpOverlapped) 
{
	
}

int 
main(int argc, char **argv) 
{
	WSADATA wd = {0};
	SOCKADDR_IN addr;
	int ret, loop = 1;
	
	// TODO: To create a default security descriptor, manual-reset, 
	// nonsignaled and without a name event for waiting the timeout stuff.  
	m_waiter = CreateEvent(NULL, true, false, NULL);
	if (m_waiter == NULL) 
	{
		printf("Fail to create event, error code is %d\n", GetLastError());
		return -1;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		printf("The event is already exists\n");
		return -1;
	}
	
	// TODO: To initiates use of WS2_32.DLL by a process.
	ret = WSAStartup(MAKEWORD(2, 0), &wd);
	if (ret != 0) 
	{
		printf("Fail to initial WinSock 2.0, error code is %d\n", 
			WSAGetLastError());
		return -1;
	}
	
	// TODO: To creates a Internet-Namespace, TCP socket that is bound to 
	// a specific service provider.
	m_listener = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listener == INVALID_SOCKET) 
	{
		printf("Fail to create socket, error code is %d\n", 
            WSAGetLastError());
		return -1;
	}
	
	// TODO: To associate a local address with a socket.
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons((short)TCP_PORT);
	ret = bind(m_listener, (SOCKADDR *)&addr, sizeof(addr));
	if (ret != 0) 
	{
		printf("Fail to bind, error code is %d\n", WSAGetLastError());
		m_cleanup();
		return -1;
	}
	
	// TODO: To place a socket a state where it is listening for an 
	// incoming connection.
	ret = listen(m_listener, SOMAXCONN);
	if (ret != 0) 
	{
		printf("Fail to listen, error code is %d\n", WSAGetLastError());
		m_cleanup();
		return -1;
	}

	// TODO: To binds the specified file handle to the thread pool's I/O 
	// completion port. On completion of the I/O request, a non-I/O worker 
	// thread will execute the callback function.
	ret = BindIoCompletionCallback((HANDLE)m_listener, 
		(LPOVERLAPPED_COMPLETION_ROUTINE)m_IOCallback, 0);
	if (ret == 0) 
	{
		printf("Fail to BindIoCompletionCallback, error code is %d\n", 
			GetLastError());
		m_cleanup();
		return -1;
	}

	printf("\nServer is running... Press q to exit...\n");
	while (loop) 
	{
		if (_kbhit()) 
		{
			switch (tolower(_getch())) 
			{
			case 'q':
				loop = 0;
				break;
			}
		}
	}

	return 0;
}
