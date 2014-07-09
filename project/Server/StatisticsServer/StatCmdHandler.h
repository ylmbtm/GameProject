#ifndef _STAT_CMD_HANDLER_H_
#define _STAT_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"

class CStatCmdHandler : public CCommonCmdHandler
{
public:
	CStatCmdHandler();

	~CStatCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL OnThreadBegin();

	BOOL OnThreadEnd();

	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdSvrRunningStateReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);


	//*********************消息处理定义结束******************************


	//日志处理函数
	BOOL OnSvrReport_NormalLog(CBufferHelper *pBufferHelper);

};

#endif //_STAT_CMD_HANDLER_H_
