#include "stdafx.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "DataBuffer/BufferHelper.h"
#include "Utility/CommonSocket.h"
#include "CommandDef.h"
#include "PacketDef/TransferPacket.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonEvent.h"
#include "Utility/CommonConvert.h"
#include "DataBuffer/DataBuffer.h"
#include "Connection.h"
#include "ConnectionType.h"

ServiceBase::ServiceBase(void)
{
}

ServiceBase::~ServiceBase(void)
{
}

BOOL ServiceBase::OnDataHandle(IDataBuffer *pDataBuffer , CConnection *pConnection)
{
	CBufferHelper BufferReader(FALSE, pDataBuffer);
	if(!BufferReader.BeginRead())
	{
		return FALSE;
	}

	CommandHeader *pCommandHeader = BufferReader.GetCommandHeader();
	if(pCommandHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(pCommandHeader->wCommandID == CMD_CONNECT_NOTIFY)
	{
		StConnectNotify ConnectNotify;
		
		BufferReader.Read(ConnectNotify);

		if(ConnectNotify.btConType == TYPE_SVR_CENTER)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[中心]-服务器【%ld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_GAME)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[游戏]-服务器【%ld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_PROXY)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[代理]-服务器【%ld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_LOGIN)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[登录]-服务器【%ld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_DATABASE)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[数据库]-服务器【%ld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_CLT_PLAYER)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[客户端]-【%x】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}

		pConnection->SetConnectionType(ConnectNotify.btConType);

		CConnectionMgr::GetInstancePtr()->SetConnectionID(pConnection, ConnectNotify.u64ConnID);

		if(!BufferReader.BeginRead())
		{
			return FALSE;
		}
	}
	
	OnCommandHandle(pCommandHeader->wCommandID, pConnection->GetConnectionID(), &BufferReader);

	return FALSE;
}

BOOL ServiceBase::StartService()
{
	CLog::GetInstancePtr()->AddLog("*******服务器信息***********");
	CLog::GetInstancePtr()->AddLog("服务器地址:%s 监听端口:%d", CGlobalConfig::GetInstancePtr()->m_strIpAddr.c_str(), CGlobalConfig::GetInstancePtr()->m_sPort);
	CLog::GetInstancePtr()->AddLog("服务器类型:%d 服务器ID:%d", CGlobalConfig::GetInstancePtr()->m_dwServerType, CGlobalConfig::GetInstancePtr()->m_dwServerID);
	CLog::GetInstancePtr()->AddLog("********服务器信息***********");
	CLog::GetInstancePtr()->AddLog("");
	CLog::GetInstancePtr()->AddLog("");

	if(!CNetManager::GetInstancePtr()->Start(this))
	{
		CLog::GetInstancePtr()->AddLog("启动网络层失败");
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("服务器启动成功!");

	return TRUE;
}

BOOL ServiceBase::StopService()
{
	CLog::GetInstancePtr()->AddLog("==========服务器开始关闭=======================");

	CNetManager::GetInstancePtr()->Close();

	CLog::GetInstancePtr()->CloseLog();

	return TRUE;
}


BOOL ServiceBase::SendCmdToConnection(UINT64 u64ConnID, IDataBuffer *pSrcBuffer )
{
	if(u64ConnID == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	IDataBuffer *pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pSrcBuffer->GetDataLenth());
	if(pSendBuffer == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pSendBuffer->CopyFrom(pSrcBuffer);

	return CNetManager::GetInstancePtr()->SendBufferByConnID(u64ConnID, pSendBuffer);
}

BOOL ServiceBase::SendCmdToConnection( UINT64 u64ConnID, UINT64 u64CharID, UINT32 dwSceneID, IDataBuffer *pSrcBuffer )
{
	if(u64ConnID == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	IDataBuffer *pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pSrcBuffer->GetDataLenth());
	if(pSendBuffer == NULL)
	{
		ASSERT_FAIELD;
		
		return FALSE;
	}

	pSendBuffer->CopyFrom(pSrcBuffer);

	CommandHeader *pCommandHeader = (CommandHeader *)(pSendBuffer->GetBuffer()+sizeof(TransferHeader));

	pCommandHeader->u64CharID	= u64CharID;
	pCommandHeader->dwSceneID	= dwSceneID;

	ASSERT(pCommandHeader->u64CharID  != 0);
	ASSERT(pCommandHeader->wCommandID != 0);


	return CNetManager::GetInstancePtr()->SendBufferByConnID(u64ConnID, pSendBuffer);
}


BOOL ServiceBase::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{	
	return TRUE;
}

BOOL ServiceBase::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	return CNetManager::GetInstancePtr()->ConnectToOtherSvrEx(strIpAddr, sPort);
}

BOOL ServiceBase::OnDisconnect( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL ServiceBase::OnUpdate( UINT32 dwTick )
{
	ASSERT_FAIELD;

	//不要处理这个方法， 基本不需要调动。

	return TRUE;
}

CConnection* ServiceBase::GetConnectionByID( UINT64 u64ConnID )
{
	return CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(u64ConnID);
}





