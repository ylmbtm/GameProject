#include "stdafx.h"
#include "DBCmdHandler.h"
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
#include "PacketDef/DBPacket.h"



CDBCmdHandler::CDBCmdHandler()
{

}

CDBCmdHandler::~CDBCmdHandler()
{

}

BOOL CDBCmdHandler::Init(UINT32 dwReserved)
{
	CCommonCmdHandler::Init(dwReserved);

	std::string strPath = CommonFunc::GetCurrentDir();
	strPath += "\\Game.DB";

	m_DBConnection.open(strPath.c_str());

	return TRUE;
}

BOOL CDBCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CDBCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_DB_NEW_ACCOUNT_REQ,	OnCmdDBNewAccountReq);
		PROCESS_COMMAND_ITEM(CMD_DB_NEW_CHAR_REQ,		OnCmdDBNewCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_PICK_CHAR_REQ,		OnCmdDBPickCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_LOGIN_REQ,			OnCmdDBLoginReq);
	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CDBCmdHandler::OnUpdate( UINT32 dwTick )
{


	return TRUE;
}


UINT32 CDBCmdHandler::OnCmdDBNewAccountReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBNewAccountReq DBNewAccountReq;
	pBufferHelper->Read(DBNewAccountReq);


	StDBNewAccountAck DBNewAccountAck;
	DBNewAccountAck.CharNewAccountAck.dwRetCode = 1;
	DBNewAccountAck.u64ConnID = DBNewAccountReq.u64ConnID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_NEW_ACCOUNT_ACK, 0, 0, 0);
	WriteHelper.Write(DBNewAccountAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return 0;
}

UINT32 CDBCmdHandler::OnCmdDBNewCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBNewCharReq DBNewCharReq;
	pBufferHelper->Read(DBNewCharReq);

	StDBCharNewCharAck DBCharNewCharAck;
	DBCharNewCharAck.u64ConnID = DBNewCharReq.u64ConnID;



	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_NEW_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharNewCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);


	return 0;
}

UINT32 CDBCmdHandler::OnCmdDBPickCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBCharPickCharReq DBCharPickCharReq;
	pBufferHelper->Read(DBCharPickCharReq);

	StDBCharPickCharAck DBCharPickCharAck;
	DBCharPickCharAck.u64ConnID = DBCharPickCharReq.u64ConnID;



	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_PICK_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharPickCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return 0;
}

UINT32 CDBCmdHandler::OnCmdDBLoginReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{	
	StDBCharLoginReq DBCharLoginReq;
	pBufferHelper->Read(DBCharLoginReq);


	StDBCharLoginAck DBCharLoginAck;
	DBCharLoginAck.u64ConnID = DBCharLoginReq.u64ConnID;

	DBCharLoginAck.CharLoginAck.byteCode = 0;



	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_LOGIN_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharLoginAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return 0;
}
