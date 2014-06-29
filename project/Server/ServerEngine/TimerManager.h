#ifndef _TIMER_MANAGER__
#define _TIMER_MANAGER__

#define ID_BLOOD  1

struct TimeEvent
{
public:
	TimeEvent();

	UINT32 dwStartTime;
	UINT32 dwEndTime;
	UINT32 dwTimerID;
	UINT32 wParam;
	UINT32 lParam;

	BYTE   RepeatTimes;

	TimeEvent *m_pNext;
};

#define BEGIN_TIMER_PROCESS VOID OnTimerEvent( TimeEvent *pEvent ){if(pEvent == NULL){return ;}switch(pEvent->dwTimerID){
#define PROCESS_TIMER_ITEM(TIMER_ID, FUNC) case TIMER_ID:{FUNC(pEvent->dwStartTime, pEvent->dwStartTime, pEvent->wParam, pEvent->lParam);}break;
#define END_TIMER_PROCESS default:{}break;}}

class TimerManager
{
public:
	TimerManager();

	~TimerManager();

public:
	BOOL AddTimer(UINT32 dwTimerID, UINT32 wParam, UINT32 lParam);

	BOOL DelTimer(UINT32 dwTimerID);

	VOID UpdateTimer();

	TimeEvent *m_pHead;

	TimeEvent *m_pFree;
 
public:
	BEGIN_TIMER_PROCESS
		PROCESS_TIMER_ITEM(ID_BLOOD, OnBlood)
	END_TIMER_PROCESS

public:

	VOID   OnBlood(UINT32 dwStartTime, UINT32 dwEndTime, UINT32 wParam, UINT32 lParam);
};



#endif /* _TIMER_MANAGER__ */
