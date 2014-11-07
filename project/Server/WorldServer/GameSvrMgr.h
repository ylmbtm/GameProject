#ifndef __GAME_SVR_MGR_
#define __GAME_SVR_MGR_

#include "PacketDef/ServerPacket.h"
#include "DataBuffer/DataBuffer.h"

class CGameSvrMgr
{
public:
	CGameSvrMgr(void);
	virtual ~CGameSvrMgr(void);

	//*********************消息处理定义开始******************************
public:
	

	//*********************消息处理定义结束******************************
public:
	UINT32	GetServerIDBySceneID(UINT32 dwSceneID);

	UINT32	MakeNewSceneID(UINT32 dwMapID);

	UINT32	GetFreeGameServerID();

	BOOL	CreateScene(UINT32 dwMapID, UINT32 CreateParam);

	BOOL	SendCreateSceneCmd(UINT32 dwServerID, UINT32 dwSceneID, UINT32 CreateParam);



public:
	CDataBuffer<CONST_BUFF_SIZE>	m_WriteBuffer;			//固定的写buffer

};

#endif