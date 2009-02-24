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

#include <winsock2.h>

//-----------------------------------------------------------------------------
//----- Private attribute.
static SOCKET m_SocketHandler;
static char *m_Host = NULL;
static int m_Port = 0;
static int m_ErrorCode = 0;

// TODO: To act as construct.
bool 
libsockclient_init(char *Host, int Port) 
{
	WSADATA wsaData;
	struct hostent *pServer;
	struct sockaddr_in pServerAddr;
	int err;

	// TODO: To initial use of WS2_32.DLL by a process.
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) 
	{
		m_ErrorCode = WSAGetLastError();
		return false;
	}

	// TODO: To create a socket that is bound to a specific transport service 
    // provider, and optionally creates and/or joins a socket group.
	m_SocketHandler = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 
        WSA_FLAG_OVERLAPPED);
	if (m_SocketHandler == INVALID_SOCKET) 
	{
		m_ErrorCode = WSAGetLastError();
		return false;
	}

	// TODO: To retrieve host information corresponding to a host name from a 
    // host database.
	pServer = gethostbyname(Host);
	if (pServer == NULL) 
	{
		libsockclient_cleanup();
		m_ErrorCode = WSAGetLastError();
		return false;
	}

	// TODO: To establishe a connection to another socket application, exchanges 
    // connect data, and specifies needed quality of service based on the supplied 
    // FLOWSPEC structure.
	ZeroMemory((char *)&pServerAddr, sizeof(pServerAddr));
	pServerAddr.sin_family = AF_INET;
	pServerAddr.sin_port = htons(Port);
	CopyMemory((char *)&pServerAddr.sin_addr.s_addr, (char *)pServer->h_addr, 
		pServer->h_length);
	if (SOCKET_ERROR == WSAConnect(m_SocketHandler, 
		reinterpret_cast<const struct sockaddr *>(&pServerAddr), sizeof(pServerAddr), 
		NULL, NULL, NULL, NULL)) 
	{
		libsockclient_cleanup();
		m_ErrorCode = WSAGetLastError();
		return false;
	}

	m_Host = Host;
	m_Port = Port;
	return true;
}

// TODO: To act as destruct.
void 
libsockclient_cleanup() 
{
	// TODO: To close an existing socket.
	closesocket(m_SocketHandler);

	// TODO: To terminate use of the WS2_32.DLL.
	WSACleanup();
}

// TODO: To send buffer to the server.
void 
libsockclient_send(char *Buffer, bool ReSend) 
{
	WSABUF DataBuf;
	DWORD dwSendBytes = 0;
	DWORD dwFlags = 0;
	int err;

	DataBuf.buf = Buffer;
	DataBuf.len = strlen(Buffer);
	err = WSASend(m_SocketHandler, &DataBuf, 1, &dwSendBytes, dwFlags, NULL, NULL);
	// TODO: To re-send.
    if (err == SOCKET_ERROR && ReSend) 
	{
		m_ErrorCode = WSAGetLastError();
		Sleep(REDO_INTERVAL);
		libsockclient_init(m_Host, m_Port);
		libsockclient_send(Buffer, true);
	}
}

// TODO: To receive buffer from the server.
void 
libsockclient_recv(char *Buffer, int BufferSize, bool ReRecv) 
{
	WSABUF DataBuf;
	DWORD dwRecvBytes;
	DWORD dwFlags = 0;
	int err;

	DataBuf.buf = Buffer;
	DataBuf.len = BufferSize;
	err = WSARecv(m_SocketHandler, &DataBuf, 1, &dwRecvBytes, &dwFlags, NULL, NULL);
	// TODO: To re-receive.
    if (err == SOCKET_ERROR && ReRecv) 
	{
		m_ErrorCode = WSAGetLastError();
		Sleep(REDO_INTERVAL);
		libsockclient_init(m_Host, m_Port);
		libsockclient_recv(Buffer, BufferSize, true);
	}
}

// TODO: To get error code.
int 
libsockclient_geterrorcode() 
{
	return m_ErrorCode;
}
