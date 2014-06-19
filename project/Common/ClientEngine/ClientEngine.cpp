#include "StdAfx.h"
#include "ClientEngine.h"
#include "CommandDef.h"
#include "PacketDef\TransferPacket.h"
#include "DataBuffer\BufferHelper.h"
#include "ConnectionType.h"
#include "PacketDef\ClientPacket.h"
#include "Error.h"

ClientEngine::ClientEngine(void)
{
	m_u64ClientID = 0;
	
	m_NetworkMgr.m_EngineMsgHandler = (ICommandHandler *)this;
}

ClientEngine::~ClientEngine(void)
{
	m_u64ClientID = 0;
}


BOOL ClientEngine::InitEngine()
{
	

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

BOOL ClientEngine::SendData( char *pData, UINT32 dwLen )
{
	if((pData == NULL)||(dwLen == 0))
	{
		return FALSE;
	}

	m_NetworkMgr.SendData(pData, dwLen);

	return TRUE;
}

BOOL ClientEngine::Login(const char *pszAccountName, const char *pszPassword)
{
	if((pszPassword == NULL)||(pszAccountName == NULL))
	{
		return FALSE;
	}

	StCharLoginReq CharLoginReq;
	strncpy(CharLoginReq.szAccountName, pszAccountName, 32);
	strncpy(CharLoginReq.szPassword, pszPassword, 32);

	CBufferHelper WriteHelper(TRUE, m_NetworkMgr.m_pWriteBuffer);

	WriteHelper.BeginWrite(CMD_CHAR_LOGIN_REQ, 0, 0, 0);

	WriteHelper.Write(CharLoginReq);

	WriteHelper.EndWrite();

	m_NetworkMgr.SendData(m_NetworkMgr.m_pWriteBuffer->GetData(), m_NetworkMgr.m_pWriteBuffer->GetDataLenth());

	return TRUE;
}

BOOL ClientEngine::RegisterMsgHandler(IMessageHandler *pMsgHandler)
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
	m_NetworkMgr.ProcessOnce();

	return TRUE;
}

BOOL ClientEngine::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CONNECT_NOTIFY,		OnCmdConnectNotify);

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_PICK_CHAR_ACK,	OnCmdPickCharAck);


	default:
		{
			for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
			{
				IMessageHandler *pHandler = *itor;

				if(pHandler->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper))
				{
					break;
				}
			}
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

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_REQ, CMDH_SENCE, 0, CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameReq);

		WriteHelper.EndWrite();

		m_NetworkMgr.SendData(m_NetworkMgr.m_pWriteBuffer->GetData(),m_NetworkMgr.m_pWriteBuffer->GetDataLenth());
	}
	else if(ConType == TYPE_SVR_LOGIN)
	{
		m_NetworkMgr.SetConnectState(Succ_Connect);
	}

	return 0;
}

ClientEngine* ClientEngine::GetInstancePtr()
{
	static ClientEngine _Handler;

	return &_Handler;
}

IDataBuffer* ClientEngine::GetWriteBuffer()
{
	return m_NetworkMgr.m_pWriteBuffer;
}

UINT32 ClientEngine::OnCmdPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharPickCharAck CharPickCharAck;
	pBufferHelper->Read(CharPickCharAck);

	if(CharPickCharAck.nRetCode == E_SUCCESSED)
	{
		m_NetworkMgr.DisConnect();
		m_u64ClientID = CharPickCharAck.u64CharID;
		m_NetworkMgr.ConnectToServer(CharPickCharAck.szIpAddr, CharPickCharAck.sPort);
	}

	return 0;
}

BOOL ClientEngine::ConnectToServer( std::string strIpAddr, UINT16 sPort )
{
	return m_NetworkMgr.ConnectToServer(strIpAddr, sPort);
}

BOOL ClientEngine::DisConnect()
{
	return m_NetworkMgr.DisConnect();
}




