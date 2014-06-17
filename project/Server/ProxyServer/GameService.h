﻿#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "CharWillEnterList.h"
class  CStaticPlayer;
class  CConnection;

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

	BOOL		RelayToServer(CStaticPlayer *pClientObj, IDataBuffer *pBuffer);

	BOOL		RelayToClient(CStaticPlayer *pClientObj, IDataBuffer *pBuffer);

public:
	//处理普通的网络连接
	CServerCmdHandler   m_ServerCmdHandler;

	CWillEnterNodeMgr   m_WillEnterNodeMgr;
};

#endif
