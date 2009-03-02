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

#include "sync.h"
#include "threadpool.h"

#include <stdio.h>

int main() 
{
    CCTEC::Mutex pMutex;

    pMutex.Lock();
    pMutex.Unlock();
    printf("%d\n", CCTEC::GetProcessorNum());
    return 0;
}

