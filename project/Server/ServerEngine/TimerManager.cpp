#include "stdafx.h"
#include "TimerManager.h"
#include "../../Common/Utility/CommonFunc.h"


TimeEvent::TimeEvent()
{
	dwStartTime = 0;
	dwEndTime   = 0;
	dwTimerID   = 0;
	wParam      = 0;
	lParam		= 0;

	m_pNext     = NULL;
}

TimerManager::TimerManager()
{
	m_pHead = NULL;

	m_pFree = NULL;
}

TimerManager::~TimerManager()
{

}

BOOL TimerManager::AddTimer( UINT32 dwTimerID, UINT32 wParam, UINT32 lParam )
{
	TimeEvent *pNewEvent = NULL;
	if(m_pFree == NULL)
	{
		pNewEvent = new TimeEvent;
	}
	else
	{
		pNewEvent = m_pFree;

		m_pFree = m_pFree->m_pNext;
	}

	pNewEvent->dwTimerID = dwTimerID = dwTimerID;

	pNewEvent->wParam = wParam;

	pNewEvent->lParam = lParam;

	if(m_pFree == NULL)
	{
		m_pFree = pNewEvent;

		return TRUE;
	}

	if(pNewEvent->dwEndTime <= m_pFree->dwEndTime)
	{
		pNewEvent->m_pNext = m_pFree;

		m_pFree = pNewEvent;

		return TRUE;
	}

	TimeEvent *pInserPos = m_pFree;

	while(pInserPos != NULL)
	{
		if(pNewEvent->dwEndTime >= pInserPos->dwEndTime)
		{
			if(pInserPos->m_pNext == NULL)
			{
				pInserPos->m_pNext = pNewEvent;

				return TRUE;
			}

			if(pNewEvent->dwEndTime >= pInserPos->m_pNext->dwEndTime)
			{
				pInserPos = pInserPos->m_pNext;
				continue;
			}

			pNewEvent->m_pNext = pInserPos->m_pNext;

			pInserPos->m_pNext = pNewEvent;

			return TRUE;
		}
	}

	return TRUE;
}

BOOL TimerManager::DelTimer( UINT32 dwTimerID )
{
	if(m_pHead == NULL)
	{
		return TRUE;
	}

	if(m_pHead->dwTimerID == dwTimerID)
	{
		TimeEvent *pEvent = m_pHead;

		m_pHead = m_pHead->m_pNext;

		pEvent->m_pNext = m_pFree->m_pNext;

		m_pFree = pEvent;

		return TRUE;
	}


	TimeEvent *pEvent = m_pHead;

	while(pEvent->m_pNext != NULL)
	{
		if(pEvent->m_pNext->dwTimerID == dwTimerID)
		{
			TimeEvent *pDelEvent = pEvent->m_pNext;

			pEvent->m_pNext = pEvent->m_pNext->m_pNext;

			pDelEvent->m_pNext = m_pFree->m_pNext;

			m_pFree = pDelEvent;

			return TRUE;
		}


		pEvent = pEvent->m_pNext;
	}

	return FALSE;
}

VOID TimerManager::UpdateTimer()
{
	UINT32 dwCurTick = CommonFunc::GetTickCount();

	while(m_pHead != NULL)
	{
		if(dwCurTick >= m_pHead->dwEndTime)
		{
			OnTimerEvent(m_pHead);

			TimeEvent *pEvent = m_pHead;

			m_pHead = m_pHead->m_pNext;

			pEvent->m_pNext = m_pFree;

			m_pFree = pEvent;
		}
	}
}


VOID TimerManager::OnBlood( UINT32 dwStartTime, UINT32 dwEndTime, UINT32 wParam, UINT32 lParam )
{

	return ;
}
