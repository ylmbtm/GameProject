#ifndef __GAME_SVR_MGR_
#define __GAME_SVR_MGR_

#include "PacketDef/ServerPacket.h"

class CGameSvrMgr
{
public:
	CGameSvrMgr(void);
	virtual ~CGameSvrMgr(void);

public:
	UINT32	GetServerIdBySceneID(UINT32 dwSceneID);

	UINT32	MakeNewSceneID(UINT32 dwMapID);

	UINT32	GetFreeGameServerID();

	BOOL	CreateScene(UINT32 dwMapID, UINT32 CreateParam);

	BOOL	SendCreateSceneCmd(UINT32 dwServerID, UINT32 dwSceneID, UINT32 CreateParam);


};

#endif