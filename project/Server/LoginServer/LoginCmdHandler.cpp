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



CLoginCmdHandler::CLoginCmdHandler()
{

}

CLoginCmdHandler::~CLoginCmdHandler()
{

}

BOOL CLoginCmdHandler::Init(UINT32 dwReserved)
{
	CCommonCmdHandler::Init(dwReserved);

	m_mapAccount.insert(std::make_pair("zhangming", CharInfo("zhangming", "123", MAKE_PLAYRE_ID(1))));
	m_mapAccount.insert(std::make_pair("test1",		CharInfo("test1", "123", MAKE_PLAYRE_ID(2))));
	m_mapAccount.insert(std::make_pair("test2",		CharInfo("test2", "123", MAKE_PLAYRE_ID(3))));

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

	stdext::hash_map<std::string, CharInfo>::iterator itor = m_mapAccount.find(MsgLoginReq.szAccountName);

	StCharLoginAck MsgLoginAck;

	MsgLoginAck.byteCode = 1;

	if(itor == m_mapAccount.end())
	{
		MsgLoginAck.byteCode = 0;
	}
	else if(MsgLoginReq.szPassword != itor->second.strPassword)
	{
		MsgLoginAck.byteCode = 0;
	}
	else
	{
		MsgLoginAck.byteCode = 1;
		MsgLoginAck.u64CharID = itor->second.dwCharID;
	}

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_CHAR_LOGIN_ACK, 0, 0, 0);

	WriteHelper.Write(MsgLoginAck);

	WriteHelper.EndWrite();

	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID,  &m_WriteBuffer);
	
	return 0;
}
