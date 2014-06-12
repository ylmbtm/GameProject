#include "stdafx.h"
#include "CommandDef.h"
#include "GameDef.h"
#include "LoginCmdHandler.h"
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

		PROCESS_COMMAND_ITEM(CMD_DB_NEW_ACCOUNT_ACK,	OnCmdDBNewAccountAck);
		PROCESS_COMMAND_ITEM(CMD_DB_NEW_CHAR_ACK,		OnCmdDBNewCharAck);
		PROCESS_COMMAND_ITEM(CMD_DB_PICK_CHAR_ACK,		OnCmdDBPickCharAck);
		PROCESS_COMMAND_ITEM(CMD_DB_LOGIN_ACK,			OnCmdDBLoginAck);

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
	WriteHelper.BeginWrite(CMD_DB_LOGIN_REQ, 0, 0, 0);
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
	WriteHelper.BeginWrite(CMD_DB_NEW_ACCOUNT_REQ, 0, 0, 0);
	WriteHelper.Write(DBNewAccountReq);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdNewCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewCharReq CharNewCharReq;
	pBufferHelper->Read(CharNewCharReq);

	StDBNewCharReq DBNewCharReq;
	DBNewCharReq.u64ConnID = u64ConnID;
	DBNewCharReq.CharNewCharReq = CharNewCharReq;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_PICK_CHAR_REQ, 0, 0, 0);
	WriteHelper.Write(DBNewCharReq);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdPickCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharPickCharReq CharPickCharReq;
	pBufferHelper->Read(CharPickCharReq);

	StDBCharPickCharReq DBPickCharReq;
	DBPickCharReq.u64ConnID = u64ConnID;
	DBPickCharReq.CharPickCharReq = CharPickCharReq;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_NEW_CHAR_REQ, 0, 0, 0);
	WriteHelper.Write(DBPickCharReq);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdDBNewAccountAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBNewAccountAck DBNewAccountAck;
	pBufferHelper->Read(DBNewAccountAck);

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEW_ACCOUNT_ACK, 0, 0, 0);
	WriteHelper.Write(DBNewAccountAck.CharNewAccountAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBNewAccountAck.u64ConnID, &m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdDBNewCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{	
	StDBCharNewCharAck DBNewCharAck;
	pBufferHelper->Read(DBNewCharAck);

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEW_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBNewCharAck.CharNewCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBNewCharAck.u64ConnID, &m_WriteBuffer);
	return 0;
}

UINT32 CLoginCmdHandler::OnCmdDBPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBCharPickCharAck DBCharPickCharAck;
	pBufferHelper->Read(DBCharPickCharAck);

	//选人这里需要做以下几件事。
	//1。将可用的proxyver地址和端口，ID发给客户端。
	//2。将登录的识别码和ID发给代理服

	DBCharPickCharAck.CharPickCharAck.dwIdentifyCode = rand()%10000;
	DBCharPickCharAck.CharPickCharAck.nRetCode  = 0;
	strncpy(DBCharPickCharAck.CharPickCharAck.szIpAddr, "127.0.0.1", 32);
	DBCharPickCharAck.CharPickCharAck.sPort		= 7200;

	StCharWillEnterGame CharWillEnterGame;
	CharWillEnterGame.dwIdentifyCode	= DBCharPickCharAck.CharPickCharAck.dwIdentifyCode;
	CharWillEnterGame.u64CharID			= DBCharPickCharAck.CharPickCharAck.u64CharID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_SVR_CHAR_WILL_ENTER, 0, 0, 0);
	WriteHelper.Write(CharWillEnterGame);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBCharPickCharAck.u64ConnID, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_CHAR_PICK_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharPickCharAck.CharPickCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBCharPickCharAck.u64ConnID, &m_WriteBuffer);

	return 0;
}

UINT32 CLoginCmdHandler::OnCmdDBLoginAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBCharLoginAck DBCharLoginAck;
	pBufferHelper->Read(DBCharLoginAck);

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_LOGIN_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharLoginAck.CharLoginAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBCharLoginAck.u64ConnID, &m_WriteBuffer);

	return 0;
}
