#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "LoginCmdHandler.h"
class CConnection;

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
	UINT64				m_u64DBConnID;
	UINT64				GetDBConnID() const;
	void				SetDBConnID(UINT64 ConnID);
	BOOL				SendCmdToDBConnection(IDataBuffer *pDataBuf);

public:
	CServerCmdHandler   m_ServerCmdHandler;
	CLoginCmdHandler	m_LoginCmdHandler;
};

#endif
