#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "StatCmdHandler.h"

class CGameService :
	public ServiceBase
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL		OnDisconnect(CConnection *pConnection);

	BOOL		StartRun();

	BOOL		OnIdle();

public:
	CServerCmdHandler   m_ServerCmdHandler;

	CStatCmdHandler		m_StatCmdHandler;
};

#endif
