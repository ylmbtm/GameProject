#ifndef _DB_CMD_HANDLER_H_
#define _DB_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"
#include "DBProcManager.h"


class CDBCmdHandler : public CCommonCmdHandler
{
public:
	CDBCmdHandler();

	~CDBCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdDBNewAccountReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBNewCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBPickCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBLoginReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdDBGetCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	//*********************消息处理定义结束******************************


	//数据库定义
	CDBProcManager   m_DBProcManager;
};

#endif //_DB_CMD_HANDLER_H_
