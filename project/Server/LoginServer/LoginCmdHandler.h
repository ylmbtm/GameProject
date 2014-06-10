#ifndef _LOGIN_CMD_HANDLER_H_
#define _LOGIN_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"


class CLoginCmdHandler : public CCommonCmdHandler
{
public:
	CLoginCmdHandler();

	~CLoginCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdNewAccountReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdNewCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdPickCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdLoginReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdDBNewAccountAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBNewCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBPickCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBLoginAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	
	//*********************消息处理定义结束******************************
};

#endif //_LOGIN_CMD_HANDLER_H_
