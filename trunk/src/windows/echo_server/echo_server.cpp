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
#define _WIN32_WINNT 0x0400             // for Windows NT 4.0 

#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "INCLUDE/server_service.h"     // for server_service template
#include "XGetopt.h"                    // for getopt

#define DEBUG 1
#define BUFF_SIZE 512
#define MAX_CONNECTIONS 10              // for concurrent connection number, 
                                        // set very small for test purpose only 
#define NO_THREADS 4                    // for working threads number in the pool
#define TIME_OUT 10                     // for the timeout routine
#define PORT 8080                       // for the default port

//---------------------------------------------------------------------------------
struct Attachment {
	volatile time_t				tmLastActionTime;
	char						sString[BUFF_SIZE];
	DWORD						dwStringSize;   // current string size

	Attachment() { Clear(); };

	bool Commit( DWORD dwBytesTransferred ) {
		DWORD dwSize = dwStringSize + dwBytesTransferred;

		if ( dwBytesTransferred <= 0 ) return false;
		if ( dwSize >= BUFF_SIZE ) return false;

		dwStringSize = dwSize;
		sString[dwStringSize] = 0;
		return true;
	};
    
	// as requested by the API of the framework
	void Clear() { memset(this, 0, sizeof(Attachment) ); };

	// as requested by the API of the framework
	void ResetTime( bool toZero ) { 
		if (toZero) tmLastActionTime = 0;
		else {
			time_t	lLastActionTime;
			time(&lLastActionTime); 
			tmLastActionTime = lLastActionTime;
		}
	};

	// as requested by the API of the framework
	long GetTimeElapsed() {
		time_t tmCurrentTime;

		if (0 == tmLastActionTime) return 0;

		time(&tmCurrentTime);
		return (tmCurrentTime - tmLastActionTime);
	};
};

//---------------------------------------------------------------------------------
typedef ClientSocket<Attachment> MyCSocket;
typedef ServerSocket<Attachment> MySSocket;
typedef IOCPSimple<Attachment> MyIOCPSimple;
typedef ISockEvent<Attachment> MyISockEvent;
typedef ServerService<Attachment> MyServerService;
//---------------------------------------------------------------------------------

class MyISockEventHandler: public MyISockEvent {
public:
	// empty method, not used
	virtual void OnClose( MyCSocket *pSocket, MYOVERLAPPED *pOverlap, 
		MySSocket *pServerSocket, MyIOCPSimple *pHIocp ) {};

	// empty method, not used
	virtual void OnPending( MyCSocket *pSocket, MYOVERLAPPED *pOverlap, 
		MySSocket *pServerSocket, MyIOCPSimple *pHIocp ) {};

	virtual void OnAccept( MyCSocket *pSocket, MYOVERLAPPED *pOverlap, 
		MySSocket *pServerSocket, MyIOCPSimple *pHIocp ) {
		int nRet;
		DWORD dwSize;
		char *temp;
        
		dwSize = BUFF_SIZE - 1;
		temp = pSocket->GetAttachment()->sString; 

		// initiate the reading with OnAccept
        nRet = pSocket->ReadFromSocket( temp, dwSize );
#if DEBUG
        printf("DEBUG at %d: OnAccept %s\n", __LINE__, temp);
#endif
        pSocket->GetAttachment()->ResetTime( false );

		if ( nRet == SOCKET_ERROR ) {
			pServerSocket->Release( pSocket );
		}
	};

	virtual void OnReadFinalized( MyCSocket *pSocket, MYOVERLAPPED *pOverlap,
		DWORD dwBytesTransferred, MySSocket *pServerSocket, MyIOCPSimple *pHIocp ) {
		int nRet;
		DWORD dwSize, dwPos, dwIndex;
		char *temp, sender[BUFF_SIZE];

		// finalize the filling of the buffer
		pSocket->GetAttachment()->Commit( dwBytesTransferred );

		dwSize = BUFF_SIZE - 1;
		dwPos = pSocket->GetAttachment()->dwStringSize;
		temp = pSocket->GetAttachment()->sString;
		
        nRet = pSocket->ReadFromSocket( temp + dwPos, dwSize - dwPos );
#if DEBUG
        printf("DEBUG at %d: OnReadFinalized %s - %d(%d)\n", __LINE__, temp, 
            strlen(temp), dwBytesTransferred);
#endif
        pSocket->GetAttachment()->ResetTime( false );

		if ( nRet == SOCKET_ERROR ) {
			pServerSocket->Release( pSocket );
		}
        // TODO: To send to the client(s), if recv buffer is not empty
		else if ( nRet != RECV_BUFFER_EMPTY ) {
            // TODO: To emulate for sending back
            memset(sender, 0, BUFF_SIZE);
            dwIndex = strlen(temp) - dwBytesTransferred;
			sprintf(sender, "server response: %s", temp + dwIndex);
            nRet = pSocket->WriteToSocket( sender, dwSize );
			if ( nRet == SOCKET_ERROR ) {
				pServerSocket->Release( pSocket );
			}
		}
	};

	virtual void OnWriteFinalized( MyCSocket *pSocket, MYOVERLAPPED *pOverlap,
		DWORD dwBytesTransferred, MySSocket *pServerSocket, MyIOCPSimple *pHIocp ) {
		char *temp = pSocket->GetAttachment()->sString;
        // clean the attachment
		//pSocket->GetAttachment()->Clear();

		// and once again
		//OnAccept(pSocket, NULL, pServerSocket, NULL);
	};
};

//---------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    int	nRet;
	MyServerService *sService;
	MyISockEventHandler *mSockHndl;
	WSAData	wsData;
    int c, max_conn = MAX_CONNECTIONS, timeout = TIME_OUT, port = PORT;
    bool blnBindLocal = false;
    const char *host = NULL;

	nRet = WSAStartup(MAKEWORD(2,2),&wsData);
	if ( nRet < 0 ) {
		Log::LogMessage(L"Can't load winsock.dll.\n");
		return -1;
	}

	// TODO: To process the command line
    printf("Usage: -h HOST_ADDRESS -t TIME_OUT -c MAX_CONNECTION -p PORT\n");
    while ((c = getopt(argc, argv, "h:t:c:p:")) != EOF) 
    {
        switch (c) 
        {
        case 'h':
            host = optarg;
            break;    
		case 't':
			timeout = atoi(optarg);
			break;
		case 'c':
			max_conn = atoi(optarg);
			break;
        case 'p':
			port = atoi(optarg);
			break;
        }    
    }
    if (!blnBindLocal && host == NULL) 
    {
        printf("ERROR! Provide host address please if without choosing bind with local!\n");
        return -1;
    }

	try {
        // TODO: To initial a pool of the MYOVERLAPPED structures, see the socket_client.h
		Overlapped::Init( max_conn );
		mSockHndl = new MyISockEventHandler();

		sService = new MyServerService((MyISockEvent *) mSockHndl, port, 
			max_conn, NO_THREADS, timeout, blnBindLocal, host);
		sService->start();

		printf("hit <ENTER> to stop ...\n");
		while( !_kbhit() ) ::Sleep(100);
		
		delete sService;
		delete mSockHndl;
	}
	catch (const char *err) {
		printf("%s\n", err);
	}
	catch (const wchar_t *err) {
		wprintf(L"%ls\n", err);
	}

	WSACleanup();
	return 0;
}

