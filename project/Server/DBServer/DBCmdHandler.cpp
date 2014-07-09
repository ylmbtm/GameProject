#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "Error.h"
#include "DBCmdHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/PacketHeader.h"
#include "PacketDef/ClientPacket.h"
#include "ConnectionType.h"
#include "GameService.h"
#include "Utility/CommonSocket.h"

#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketDef/DBPacket.h"




CDBCmdHandler::CDBCmdHandler()
{

}

CDBCmdHandler::~CDBCmdHandler()
{

}

BOOL CDBCmdHandler::Init(UINT32 dwReserved)
{
	if(!CCommonCmdHandler::Init(dwReserved))
	{
		return FALSE;
	}

	if(!m_DBProcManager.Init())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDBCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	m_DBProcManager.Uninit();

	return TRUE;
}

BOOL CDBCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_DB_NEW_ACCOUNT_REQ,	OnCmdDBNewAccountReq);
		PROCESS_COMMAND_ITEM(CMD_DB_NEW_CHAR_REQ,		OnCmdDBNewCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_DEL_CHAR_REQ,		OnCmdDBDelCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_PICK_CHAR_REQ,		OnCmdDBPickCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_LOGIN_REQ,			OnCmdDBLoginReq);
		PROCESS_COMMAND_ITEM(CMD_DB_LOAD_CHAR_REQ,		OnCmdDBLoadCharReq);
		PROCESS_COMMAND_ITEM(CMD_DB_SAVE_CHAR_REQ,		OnCmdDBSaveCharReq);
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

BOOL CDBCmdHandler::OnThreadBegin()
{

	return TRUE;
}

BOOL CDBCmdHandler::OnThreadEnd()
{
	return TRUE;
}



BOOL CDBCmdHandler::OnCmdDBNewAccountReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBNewAccountReq DBNewAccountReq;
	pBufferHelper->Read(DBNewAccountReq);

	StDBNewAccountAck DBNewAccountAck;

	if(m_DBProcManager.CreateAccount(DBNewAccountReq.CharNewAccountReq.szAccountName, DBNewAccountReq.CharNewAccountReq.szPassword))
	{
		DBNewAccountAck.CharNewAccountAck.nRetCode = E_SUCCESSED;
	}
	else
	{
		DBNewAccountAck.CharNewAccountAck.nRetCode = E_FAILED;
	}

	DBNewAccountAck.u64ConnID = DBNewAccountReq.u64ConnID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_NEW_ACCOUNT_ACK, 0, 0, 0);
	WriteHelper.Write(DBNewAccountAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return TRUE;
}

BOOL CDBCmdHandler::OnCmdDBNewCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBNewCharReq DBNewCharReq;
	pBufferHelper->Read(DBNewCharReq);

	StDBCharNewCharAck DBCharNewCharAck;
	DBCharNewCharAck.u64ConnID = DBNewCharReq.u64ConnID;

	if(!m_DBProcManager.CreateNewChar(DBNewCharReq.CharNewCharReq, DBCharNewCharAck.CharNewCharAck))
	{
		DBCharNewCharAck.CharNewCharAck.nRetCode = E_FAILED;
	}
	else
	{
		DBCharNewCharAck.CharNewCharAck.nRetCode = E_SUCCESSED;
	}

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_NEW_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharNewCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return TRUE;
}

BOOL CDBCmdHandler::OnCmdDBPickCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBCharPickCharReq DBCharPickCharReq;
	pBufferHelper->Read(DBCharPickCharReq);

	StDBCharPickCharAck DBCharPickCharAck;
	DBCharPickCharAck.u64ConnID = DBCharPickCharReq.u64ConnID;
	DBCharPickCharAck.CharPickCharAck.u64CharID = DBCharPickCharReq.CharPickCharReq.u64CharID;
	DBCharPickCharAck.CharPickCharAck.nRetCode = E_SUCCESSED;


	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_PICK_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharPickCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return TRUE;
}

BOOL CDBCmdHandler::OnCmdDBLoginReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{	
	StDBCharLoginReq DBCharLoginReq;
	pBufferHelper->Read(DBCharLoginReq);

	StDBCharLoginAck DBCharLoginAck;
	DBCharLoginAck.u64ConnID = DBCharLoginReq.u64ConnID;
	
	UINT32 dwAccountID = m_DBProcManager.VerifyAccount(DBCharLoginReq.CharLoginReq.szAccountName, DBCharLoginReq.CharLoginReq.szPassword);
	if(dwAccountID == 0)
	{
		DBCharLoginAck.CharLoginAck.nRetCode = E_FAILED;
	}
	else
	{
		m_DBProcManager.LoadAccountCharInfo(dwAccountID, DBCharLoginAck.CharLoginAck);
		DBCharLoginAck.CharLoginAck.nRetCode = E_SUCCESSED;
		DBCharLoginAck.CharLoginAck.dwAccountID = dwAccountID;

		//如果这个己有登录角色，则直接把原来的踢掉
	}

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_LOGIN_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharLoginAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return TRUE;
}

BOOL CDBCmdHandler::OnCmdDBDelCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBDelCharReq DBDelCharReq;
	pBufferHelper->Read(DBDelCharReq);

	StDBCharDelCharAck DBCharDelCharAck;
	DBCharDelCharAck.u64ConnID = DBDelCharReq.u64ConnID;

	if(!m_DBProcManager.DelChar(DBDelCharReq.CharDelCharReq))
	{
		DBCharDelCharAck.CharDelCharAck.nRetCode = E_FAILED;
	}
	else
	{
		DBCharDelCharAck.CharDelCharAck.nRetCode = E_SUCCESSED;
	}

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_DEL_CHAR_ACK, 0, 0, 0);
	WriteHelper.Write(DBCharDelCharAck);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);
	return TRUE;
}


BOOL CDBCmdHandler::OnCmdDBLoadCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBLoadCharInfoReq DBLoadCharInfoReq;
	pBufferHelper->Read(DBLoadCharInfoReq);


	StDBLoadCharInfoAck DBLoadCharInfoAck;
	DBLoadCharInfoAck.dwProxySvrID = DBLoadCharInfoReq.dwProxySvrID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_LOAD_CHAR_ACK, 0, DBLoadCharInfoReq.dwSceneID, DBLoadCharInfoReq.u64CharID);

	WriteHelper.Write(DBLoadCharInfoAck);

	CDBPlayerObject *pDBPlayer = m_DBPlayerMgr.GetPlayer(DBLoadCharInfoReq.u64CharID);
	if(pDBPlayer == NULL)
	{
		//读取一条记录，
		//读出成功
		pDBPlayer = m_DBPlayerMgr.InsertAlloc(DBLoadCharInfoReq.u64CharID);

		pDBPlayer->Init();

		pDBPlayer->m_u64ObjectID = DBLoadCharInfoReq.u64CharID;

		CHAR szSql[MAX_PATH];
		sprintf(szSql, "select * from t_charinfo where F_CharID = '%lld'", DBLoadCharInfoReq.u64CharID);

		CppSQLite3Query QueryRes = m_DBProcManager.m_DBConnection.execQuery(szSql);

		if(!QueryRes.eof())
		{
			pDBPlayer->m_dwFeature = QueryRes.getIntField("F_Feature", 0);
			strncpy(pDBPlayer->m_szObjectName, QueryRes.getStringField("F_Name", ""), 32);
			pDBPlayer->m_dwLevel = QueryRes.getIntField("F_Level", 0);
	
		}

		//if(!pDBPlayer->LoadFromDB())
		//{
		//	return TRUE;
		//}
	}

	pDBPlayer->WriteToPacket(&WriteHelper);

	WriteHelper.EndWrite();
	
	CGameService::GetInstancePtr()->SendCmdToConnection(DBLoadCharInfoReq.dwGameSvrID, &m_WriteBuffer);
	
	return TRUE;
}


BOOL CDBCmdHandler::OnCmdDBSaveCharReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{

	return TRUE;
}

