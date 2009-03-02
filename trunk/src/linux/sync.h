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

#ifndef __SYNC_H__
#define __SYNC_H__

#include <pthread.h>

namespace CCTEC 
{
    // TODO: Mutex object
    class Mutex 
    {
    private:
        // TODO: Mutex handler.
        pthread_mutex_t m_MutexHandler;

    public:
        // TODO: Construct.
        Mutex() { pthread_mutex_init(&m_MutexHandler, NULL); };
        // TODO: Destruct.
        ~Mutex() { pthread_mutex_destroy(&m_MutexHandler); };
        // TODO: To lock the mutex resource.
        void Lock() { pthread_mutex_lock(&m_MutexHandler); };
        // TODO: To unlock the mutex resource.
        void Unlock() { pthread_mutex_unlock(&m_MutexHandler); };
    };
};

#endif // __SYNC_H__

