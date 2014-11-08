#ifndef __COMMON_MESSAGE_QUEUE_
#define __COMMON_MESSAGE_QUEUE_
#include "CritSecNotify.h"

namespace CommonQueue
{
	template <typename ELEM_T, INT32 dwSize=512>
	class CMessageQueue
	{
	public:
		CMessageQueue()
		{
			nReadPos = 0;

			nWritePos = 0;
		}

		~CMessageQueue()
		{
		}

		BOOL Pop(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(nReadPos == nWritePos)
			{
				m_CritSec.Wait();
				return FALSE;
			}

			_Value = m_vtData[nReadPos];

			nReadPos = (nReadPos+1)%dwSize;
			
			m_CritSec.Unlock();
			
			return TRUE;
		}

		void Push(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(((nWritePos + 1)%dwSize) == nReadPos)
			{
				m_CritSec.Unlock();
				return ;
			}

			m_vtData[nWritePos] = _Value;

			nWritePos = (nWritePos+1)%dwSize;

			m_CritSec.Unlock();

			m_CritSec.Notify();

			return ; 
		}

		ELEM_T m_vtData[dwSize];

		INT32 nWritePos;
		
		INT32 nReadPos;

		CCritSecNotify m_CritSec;
	};

}

#endif /* __COMMON_MESSAGE_QUEUE_ */
