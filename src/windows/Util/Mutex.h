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

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <windows.h>

namespace CCTEC 
{
    class Mutex 
    {
    public:
		Mutex();
		~Mutex();
		void Lock();
		void Unlock();

	private:
		CRITICAL_SECTION m_Cs;	// Critical-section object
    };
};

#endif // __MUTEX_H__
