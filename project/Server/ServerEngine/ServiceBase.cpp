#include "stdafx.h"
#include "ServiceBase.h"
#include "NetManager.h"
#include "DataBuffer/BufferHelper.h"
#include "Utility/CommonSocket.h"
#include "CommandDef.h"
#include "PacketDef/ClientPacket.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonEvent.h"
#include "Utility/CommonConvert.h"
#include "DataBuffer/DataBuffer.h"
#include "Connection.h"
#include "ConnectionType.h"

ServiceBase::ServiceBase(void)
{
	m_pPacketDispatcher = NULL;
}

ServiceBase::~ServiceBase(void)
{
}

ServiceBase* ServiceBase::GetInstancePtr()
{
	static ServiceBase _ServiceBase;

	return &_ServiceBase;
}


BOOL ServiceBase::OnDataHandle(IDataBuffer *pDataBuffer , CConnection *pConnection)
{
	CBufferHelper BufferReader(FALSE, pDataBuffer);
	if(!BufferReader.BeginRead())
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	PacketHeader *pPacketHeader = BufferReader.GetPacketHeader();
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(pPacketHeader->wCommandID == CMD_CONNECT_NOTIFY)
	{
		StConnectNotify ConnectNotify;
		
		BufferReader.Read(ConnectNotify);

		if(ConnectNotify.btConType == TYPE_SVR_CENTER)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[中心]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_GAME)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[游戏]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_PROXY)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[代理]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_LOGIN)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[登录]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_DATABASE)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[数据库]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_CLT_PLAYER)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[客户端]-【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_STATISTICS)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[统计]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}
		else if(ConnectNotify.btConType == TYPE_SVR_WORLDSVR)
		{
			CLog::GetInstancePtr()->AddLog("收到身份验证包:来自-[世界]-服务器【%lld】, 服务器类型:%d",  ConnectNotify.u64ConnID, ConnectNotify.btConType);
		}

		if(pConnection->GetConnectionID() != 0)
		{
			ASSERT_FAIELD;
			pConnection->Close();
			CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
			
			return FALSE;
		}

		pConnection->SetConnectionType(ConnectNotify.btConType);

        if(!BufferReader.BeginRead())
        {
            ASSERT_FAIELD;
            pConnection->Close();
            CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
            return FALSE;
        }
    }

	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		pConnection->Close();
		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
		return FALSE;
	}

	NetPacket item;
	item.m_pDataBuffer = NULL;
	item.m_pConnect = pConnection;
	m_pPacketDispatcher->OnNewConnect(pConnection);
	

	m_pPacketDispatcher->DispatchPacket(&item);
	
	return TRUE;
}

BOOL ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher *pDispather)
{
	if (pDispather == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_pPacketDispatcher = pDispather;

	if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this))
	{
		CLog::GetInstancePtr()->AddLog("启动网络层失败!");
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("服务器启动成功!");

	return TRUE;
}

BOOL ServiceBase::StopNetwork()
{
	CLog::GetInstancePtr()->AddLog("==========服务器开始关闭=======================");

	CNetManager::GetInstancePtr()->Close();

	CLog::GetInstancePtr()->CloseLog();

	return TRUE;
}

template<typename T>
BOOL ServiceBase::SendCmdToConnection(UINT16 uCmdID, T &Data, UINT32 dwConnID, UINT64 uCharID,UINT32 dwSceneID)
{
	if(dwConnID == 0)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	CBufferHelper WriteHelper(TRUE, sizeof(T));
	WriteHelper.BeginWrite(uCmdID, dwSceneID, uCharID);
	WriteHelper.Write(Data);
	WriteHelper.EndWrite();

	return CNetManager::GetInstancePtr()->SendBufferByConnID(dwConnID, WriteHelper.GetDataBuffer());
}


BOOL ServiceBase::SendCmdToConnection(UINT32 dwConnID, IDataBuffer *pSrcBuffer )
{
	if(dwConnID == 0)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	IDataBuffer *pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pSrcBuffer->GetTotalLenth());
	if(pSendBuffer == NULL)
	{
		return FALSE;
	}
	pSendBuffer->CopyFrom(pSrcBuffer);

	return CNetManager::GetInstancePtr()->SendBufferByConnID(dwConnID, pSrcBuffer);
}

 BOOL ServiceBase::SendCmdToConnection( UINT32 dwConnID, UINT64 u64CharID, UINT32 dwSceneID, IDataBuffer *pSrcBuffer )
{
	if(dwConnID == 0)
	{
		ASSERT_FAIELD;
 		return FALSE;
 	}
 
 	IDataBuffer *pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pSrcBuffer->GetTotalLenth());
 	if(pSendBuffer == NULL)
 	{
 		ASSERT_FAIELD;
 		
 		return FALSE;
 	}
 
 	pSendBuffer->CopyFrom(pSrcBuffer);
 
 	PacketHeader *pPacketHeader = (PacketHeader *)(pSendBuffer->GetBuffer());

	pPacketHeader->u64CharID	= u64CharID;
 	pPacketHeader->dwSceneID	= dwSceneID;

	ASSERT(pPacketHeader->u64CharID  != 0);
	ASSERT(pPacketHeader->wCommandID != 0);

	return CNetManager::GetInstancePtr()->SendBufferByConnID(dwConnID, pSendBuffer);
 }


CConnection* ServiceBase::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	return CNetManager::GetInstancePtr()->ConnectToOtherSvrEx(strIpAddr, sPort);
}

BOOL ServiceBase::OnCloseConnect( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

BOOL ServiceBase::OnNewConnect( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

CConnection* ServiceBase::GetConnectionByID( UINT32 dwConnID )
{
	return CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
}




