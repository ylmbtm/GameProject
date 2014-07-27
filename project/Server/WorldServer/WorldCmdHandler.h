#ifndef _WORLD_CMD_HANDLER_H_
#define _WORLD_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"

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


	//*********************消息处理定义结束******************************
};

#endif //_WORLD_CMD_HANDLER_H_
