#ifndef _DB_CMD_HANDLER_H_
#define _DB_CMD_HANDLER_H_
#include "CmdHandler/CommonCmdHandler.h"
#include "PacketDef/ServerPacket.h"
#include "DBProcManager.h"
#include "DBPlayerObject.h"


class CDBCmdHandler : public CCommonCmdHandler
{
public:
	CDBCmdHandler();

	~CDBCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate( UINT32 dwTick );

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL OnThreadBegin();

	BOOL OnThreadEnd();

	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdDBNewAccountReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBNewCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBDelCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBPickCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBLoginReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBLoadCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBSaveCharReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	//*********************消息处理定义结束******************************

	//玩家数据管理器
	CDBPlayerObjectMgr  m_DBPlayerMgr;

	//数据库定义
	CDBProcManager   m_DBProcManager;
};

#endif //_DB_CMD_HANDLER_H_
