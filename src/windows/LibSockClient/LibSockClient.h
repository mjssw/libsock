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

#ifndef __LIBSOCKCLIENT_H__
#define __LIBSOCKCLIENT_H__

bool libsockclient_init(char *host, int port);
void libsockclient_cleanup();
void libsockclient_send(char *Buffer, bool resend);
void libsockclient_recv(char *Buffer, bool rerecv);
int libsockclient_geterrorcode();

#endif // __LIBSOCKCLIENT_H__
