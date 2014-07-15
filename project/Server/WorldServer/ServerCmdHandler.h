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
	BOOL OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnCmdDisConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnCmdActiveSvrList(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	//*********************消息处理定义结束******************************

	std::map<UINT64, StSvrServerInfo> m_ConnectedSvrList;	//己连接的服务器列表

	std::map<UINT64, StSvrServerInfo> m_WaitConSvrList;	//待连接的服务器列表

	BOOL m_bConnectToCenter;

	UINT32 m_dwLastTry;
};

#endif //_SERVER_CMD_HANDLER_H_
