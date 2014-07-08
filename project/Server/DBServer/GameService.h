#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "DBCmdHandler.h"
class CConnection;

#define DB_THREAD_NUM 10

class CGameService :
	public ServiceBase
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

public:
	BOOL		OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL		OnDisconnect(CConnection *pConnection);

	BOOL		StartRun();

	BOOL		OnIdle();

public:
	CServerCmdHandler   m_ServerCmdHandler;


	CDBCmdHandler		m_DBCmdHandler[DB_THREAD_NUM];
};

#endif
