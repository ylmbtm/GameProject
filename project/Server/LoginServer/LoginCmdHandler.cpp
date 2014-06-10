#include "stdafx.h"
#include "LoginCmdHandler.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/TransferPacket.h"
#include "ConnectionType.h"
#include "GameService.h"

#include "Utility/CommonSocket.h"
#include "PacketHeader.h"
#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketDef/LoginPacket.h"
#include "CommonDef.h"
#include "PacketDef/ClientPacket.h"
#include "PacketDef/DBPacket.h"



CLoginCmdHandler::CLoginCmdHandler()
{

}

CLoginCmdHandler::~CLoginCmdHandler()
{

}

BOOL CLoginCmdHandler::Init(UINT32 dwReserved)
{
	CCommonCmdHandler::Init(dwReserved);

	return TRUE;
}

BOOL CLoginCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CLoginCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_CHAR_NEW_ACCOUNT_REQ,	OnCmdNewAccountReq);
		PROCESS_COMMAND_ITEM(CMD_CHAR_NEW_CHAR_REQ,		OnCmdNewCharReq);
		PROCESS_COMMAND_ITEM(CMD_CHAR_PICK_CHAR_REQ,	OnCmdPickCharReq);
		PROCESS_COMMAND_ITEM(CMD_CHAR_LOGIN_REQ,		OnCmdLoginReq);

	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CLoginCmdHandler::OnUpdate( UINT32 dwTick )
{


	return TRUE;
}


UINT32 CLoginCmdHandler::OnCmdLoginReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLoginReq MsgLoginReq;

	pBufferHelper->Read(MsgLoginReq);

	StDBCharLoginReq DBCharLoginReq;
	DBCharLoginReq.u64ConnID = u64ConnID;
	DBCharLoginReq.CharLoginReq = MsgLoginReq;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_DB_LOGIN_ACK, 0, 0, 0);

	WriteHelper.Write(DBCharLoginReq);

	WriteHelper.EndWrite();

	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);
	
	return 0;
}

UINT32 CLoginCmdHandler::OnCmdNewAccountReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewAccountReq CharNewAccountReq;
	pBufferHelper->Read(CharNewAccountReq);

	StDBNewAccountReq DBNewAccountReq;
	DBNewAccountReq.u64ConnID = u64ConnID;
	DBNewAccountReq.CharNewAccountReq = CharNewAccountReq;
	

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_DB_NEW_ACCOUNT_ACK, 0, 0, 0);

	WriteHelper.Write(DBNewAccountReq);

	WriteHelper.EndWrite();

	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdNewCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdPickCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{

	return 0;
}
