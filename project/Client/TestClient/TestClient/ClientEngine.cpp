#include "StdAfx.h"
#include "ClientEngine.h"
#include "CommandDef.h"
#include "PacketDef\TransferPacket.h"
#include "DataBuffer\BufferHelper.h"
#include "ConnectionType.h"
#include "PacketDef\ClientPacket.h"

ClientEngine::ClientEngine(void)
{
	m_u64ClientID = 0;
}

ClientEngine::~ClientEngine(void)
{
	m_u64ClientID = 0;
}


BOOL ClientEngine::InitEngine()
{
	m_NetworkMgr.m_EngineMsgHandler = (ICommandHandler *)this;

	return TRUE;
}

BOOL ClientEngine::CloseEngine()
{
	m_NetworkMgr.m_EngineMsgHandler = NULL;

	return TRUE;
}

BOOL ClientEngine::SetClientID( UINT64 u64ClientID )
{
	m_u64ClientID = u64ClientID;

	return TRUE;
}

BOOL ClientEngine::SetLoginSvrInfo( char *szIpAddr, UINT16 sPort )
{
	if((szIpAddr == NULL)||(sPort == 0))
	{
		return FALSE;
	}

	m_strLoginSvrIp = szIpAddr;
	m_sLoginSvrPort = sPort;

	return TRUE;
}

BOOL ClientEngine::SendData( char *pData, UINT32 dwLen )
{

	return TRUE;
}

BOOL ClientEngine::Login( char *pszAccountName, char *pszPassword )
{
	if(!m_NetworkMgr.ConnectToServer(m_strLoginSvrIp, m_sLoginSvrPort))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ClientEngine::RegisterNetHandler(IMessageHandler *pMsgHandler)
{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		if(pMsgHandler == *itor)
		{
			return FALSE;
		}
	}
		
	m_vtMsgHandler.push_back(pMsgHandler);

	return TRUE;
}

BOOL ClientEngine::Render()
{
	m_NetworkMgr.ReceiveData();

	m_NetworkMgr.ProcessData();

	return TRUE;
}

BOOL ClientEngine::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CONNECT_NOTIFY,		OnCmdConnectNotify);


	default:
		{

		}
		break;
	}

	return TRUE;
}


UINT32 ClientEngine::OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	StConnectNotify ConnectNotify;

	pBufferHelper->Read(ConnectNotify);

	UINT32 ConType = ConnectNotify.btConType;

	ConnectNotify.btConType =  TYPE_CLT_PLAYER;

	ConnectNotify.u64ConnID = m_u64ClientID;

	CBufferHelper WriteHelper(TRUE, m_NetworkMgr.m_pWriteBuffer);

	WriteHelper.BeginWrite(CMD_CONNECT_NOTIFY, CMDH_SVR_CON, 0, 0);

	WriteHelper.Write(ConnectNotify);

	WriteHelper.EndWrite();

	m_NetworkMgr.SendData(m_NetworkMgr.m_pWriteBuffer->GetData(), m_NetworkMgr.m_pWriteBuffer->GetDataLenth());

	if(ConType == TYPE_SVR_PROXY)
	{
		StCharEnterGameReq CharEnterGameReq;

		CharEnterGameReq.u64CharID = m_u64ClientID;

		CBufferHelper WriteHelper(TRUE, m_NetworkMgr.m_pWriteBuffer);

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_REQ, CMDH_SENCE, 12, CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameReq);

		WriteHelper.EndWrite();

		m_NetworkMgr.SendData(m_NetworkMgr.m_pWriteBuffer->GetData(),m_NetworkMgr.m_pWriteBuffer->GetDataLenth());
	}
	else if(ConType == TYPE_SVR_LOGIN)
	{
		StCharLoginReq CharLoginReq;
		strncpy(CharLoginReq.szAccountName, m_strAccountName.c_str(), 32);
		strncpy(CharLoginReq.szPassword, m_strPassword.c_str(), 32);

		CBufferHelper WriteHelper(TRUE, m_NetworkMgr.m_pWriteBuffer);

		WriteHelper.BeginWrite(CMD_CHAR_LOGIN_REQ, 0, 0, 0);

		WriteHelper.Write(CharLoginReq);

		WriteHelper.EndWrite();

		m_NetworkMgr.SendData(m_NetworkMgr.m_pWriteBuffer->GetData(), m_NetworkMgr.m_pWriteBuffer->GetDataLenth());
	}

	return 0;
}




