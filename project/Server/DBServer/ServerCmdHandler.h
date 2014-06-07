#ifndef _SERVER_CMD_HANDLER_H_
#define _SERVER_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"


class CServerCmdHandler : public CCommonCmdHandler
{
public:
	CServerCmdHandler();

	~CServerCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdDisConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdTestMsg(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdActiveSvrList(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	//*********************消息处理定义结束******************************

	std::map<UINT64, StRegisterToCenterSvr> m_vtActiveSvrList;
	BOOL m_bConnectToCenter;
	UINT32 m_dwLastTry;
};

#endif //_SERVER_CMD_HANDLER_H_
