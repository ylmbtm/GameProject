#ifndef _WORLD_CMD_HANDLER_H_
#define _WORLD_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"
#include "GameSvrMgr.h"
#include "PlayerObject.h"

class CWorldCmdHandler : public CCommonCmdHandler
{
public:
	CWorldCmdHandler();

	~CWorldCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdEnterGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBLoadCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdCreateSceneAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdLeaveGameReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	//*********************消息处理定义结束******************************


	CGameSvrMgr			m_GameSvrMgr;

	CPlayerObjectMgr    m_PlayerObjectMgr;
};

#endif //_WORLD_CMD_HANDLER_H_
