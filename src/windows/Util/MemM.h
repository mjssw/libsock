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

#ifndef __MEMM_H__
#define __MEMM_H__

#include "Mutex.h"

namespace CCTEC 
{
	template<class T> 
		class QueuedBlocks 
	{
	public:
		QueuedBlocks(int nInitSize = 1) 
		{
			int i;
			int nSize = (nInitSize <= 0) ? 1 : nInitSize;

			for (i = 0; i < nSize; i++) 
			{
				T *t = new T();
				if (t) 
				{
					
				}
			}
		}
		~QueuedBlocks() 
		{
			// TODO: To lock the critical section, but are there multi-threads 
			// to cleanup the object?
			m_Mutex.Lock();
			m_Mutex.UnLock();
		}

	private:
		Mutex m_Mutex;
	};
};

#endif // __MEMM_H__