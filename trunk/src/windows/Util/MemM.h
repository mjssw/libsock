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

#include <set>		// for std::set template
#include <vector>	// for std::vector template

namespace CCTEC 
{
	// TODO: A template class allowing re-using of the memory blocks.
	template<class T> 
		class QueuedBlocks 
	{
		// Operation
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
					m_quBlocks.insert(t);
				}
			}
		}

		~QueuedBlocks() 
		{
			// TODO: To lock the critical section, but are there multi-threads 
			// to cleanup the object?
			m_Mutex.Lock();
			m_quBlocks.clear();
			std::vector<T *>::iterator iter = m_allBlocks.begin();
			for (; iter < m_allBlocks.end(); iter++) 
			{
				T *t = *iter;
				delete t;
			}
			m_allBlocks.clear();
			m_Mutex.Unlock();
		}

		// TODO: To get a free block from the queue, if one cannot be found
		// then NULL is returned.
		T * GetFromQueue() 
		{
			T *t = NULL;

			m_Mutex.Lock();
			if (!m_quBlocks.empty()) 
			{
				std::set<T *>::iterator iter = m_quBlocks.begin();
				t = *iter;
				m_quBlocks.erase(t);
			}
			m_Mutex.Unlock();
			return t;
		}

		// TODO: To get a free block from the queue, if one cannot be found
		// then a new one is created
		T* Get() 
		{
			T* t = GetFromQueue();
			if (t == NULL) 
			{
				t = new T();
				if (t) 
				{
					m_Mutex.Lock();
					m_allBlocks.push_back(t);
					m_Mutex.Unlock();
				}
			}
			return t;
		};

		// TODO: To release the used block, place it back to the queue. 
		// For performance reason, we assume that the block was previously 
		// taken from the queue.
		void Release(T *t) 
		{
			if (t) 
			{
				m_Mutex.Lock();
				m_quBlocks.insert(t);
				m_qMutex.Unlock();
			}
		};

		// TODO: To return all the blocks ever allocated.
		std::vector<T *> *GetBlocks() 
		{ 
			return &m_allBlocks; 
		};

		// Attribute
	private:
		Mutex m_Mutex;
		std::set<T *> m_quBlocks;
		std::vector<T *> m_allBlocks;
	};

	// TODO: A template class used for providing free blocks as well 
	// as releasing unnecessary ones. Uses "QueuedBlocks" which 
	// allows reusing of the blocks.
	template<class T> 
		class StaticBlocks 
	{
		// Operation
	public:
		static void Init(int nSize = 1) 
		{
			if (blocks == NULL) blocks = new QueuedBlocks<T>(nSize);
		};

		static T *Get() 
		{
			if (blocks == NULL) return NULL;
			return blocks->Get();
		};

		static void Release(T *b) 
		{
			if (blocks != NULL) blocks->Release(b);
		};

		// Attribute
	private:
		static QueuedBlocks<T> *blocks;
	};
};

#endif // __MEMM_H__
