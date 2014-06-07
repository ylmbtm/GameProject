#include "stdafx.h"
#include "ServerCmdHandler.h"
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



CServerCmdHandler::CServerCmdHandler()
{
	
}

CServerCmdHandler::~CServerCmdHandler()
{
	
}

BOOL CServerCmdHandler::Init(UINT32 dwReserved)
{
	CCommonCmdHandler::Init(dwReserved);

	return TRUE;
}

BOOL CServerCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CServerCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_CONNECT_NOTIFY,		OnCmdConnectNotify);

		PROCESS_COMMAND_ITEM(CMD_DISCONNECT_NOTIFY,		OnCmdDisConnectNotify);

		PROCESS_COMMAND_ITEM(CMD_REGISTER_TO_CENTER,	OnCmdRegisterToCenter);//注册自己到中心服务器

		PROCESS_COMMAND_ITEM(CMD_ACTIVE_SERVER_LIST,	OnCmdActiveSvrList);//中心服务器来的数据

	default:
		{

		}
		break;
	}

	return TRUE;
}


UINT32 CServerCmdHandler::OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	StConnectNotify ConnectNotify;

	pBufferHelper->Read(ConnectNotify);

	//如果是中心服务器，就先发请求其它服务器信息和注删自己的信息
	if(ConnectNotify.btConType == TYPE_SVR_CENTER)
	{
		// 发送其它服务器的请求信息	

		StRegisterToCenterSvr RegisterToCenterSvr;

		RegisterToCenterSvr.dwSvrID = CGlobalConfig::GetInstancePtr()->m_dwServerID;
		RegisterToCenterSvr.dwType = CGlobalConfig::GetInstancePtr()->m_dwServerType;
		strncpy(RegisterToCenterSvr.szIpAddr, CGlobalConfig::GetInstancePtr()->m_strIpAddr.c_str(), 32);
		RegisterToCenterSvr.sPort = CGlobalConfig::GetInstancePtr()->m_sPort;

		CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

		WriteHelper.BeginWrite(CMD_REGISTER_TO_CENTER, CMDH_SVR_CON, 0, 0);

		WriteHelper.Write(RegisterToCenterSvr);

		WriteHelper.EndWrite();

		CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);
	}

	return 0;
}

UINT32 CServerCmdHandler::OnCmdRegisterToCenter(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_ACTIVE_SERVER_LIST, CMDH_SVR_CON, 0, 0);

	int nCount = m_vtActiveSvrList.size()>10?10:m_vtActiveSvrList.size();

	WriteHelper.Write(nCount);

	for(std::map<UINT64, StRegisterToCenterSvr>::iterator itor = m_vtActiveSvrList.begin(); itor != m_vtActiveSvrList.end(); itor++)
	{
		WriteHelper.Write(itor->second);
	}

	WriteHelper.EndWrite();
	
	CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	StRegisterToCenterSvr RegisterToCenterSvr; 

	pBufferHelper->Read(RegisterToCenterSvr);

	m_vtActiveSvrList.insert(std::make_pair(u64ConnID,RegisterToCenterSvr));

	return 0;
}

UINT32 CServerCmdHandler::OnCmdActiveSvrList(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	UINT32 dwCount = 0;

	m_vtActiveSvrList.clear();

	pBufferHelper->Read(dwCount);

	StRegisterToCenterSvr RegisterToCenterSvr;
	for(UINT32 i = 0; i < dwCount; i++)
	{
		pBufferHelper->Read(RegisterToCenterSvr);

		if(RegisterToCenterSvr.dwSvrID == CGlobalConfig::GetInstancePtr()->m_dwServerID)
		{
			continue;
		}

		CConnection *pConn = CGameService::GetInstancePtr()->GetConnectionByID(RegisterToCenterSvr.dwSvrID);
		if(pConn == NULL)
		{
			CGameService::GetInstancePtr()->ConnectToOtherSvr(RegisterToCenterSvr.szIpAddr,RegisterToCenterSvr.sPort);
		}
		else
		{
			ASSERT_FAIELD;
		}


		m_vtActiveSvrList.insert(std::make_pair(u64ConnID,RegisterToCenterSvr));
	}

	return 0;
}

BOOL CServerCmdHandler::OnUpdate( UINT32 dwTick )
{
	return TRUE;
}

UINT32 CServerCmdHandler::OnCmdDisConnectNotify( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	m_vtActiveSvrList.erase(u64ConnID);

	return 0;
}
