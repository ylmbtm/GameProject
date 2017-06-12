#include "stdafx.h"
#include "GameService.h"
#include "ObjectID.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "PacketDef/ClientPacket.h"
#include "Utility/Position.h"

CGameService::CGameService(void)
{
	m_dwWorldConnID = 0;
}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}


BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("ProxyServer","log"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("ProxyServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn,this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");
		return FALSE;
	}

	if(!m_ServerCmdHandler.Init(0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(!m_ProxyCmdHandler.Init(0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

CCommonEvent ComEvent;

#ifdef WIN32
BOOL WINAPI HandlerCloseEvent(DWORD dwCtrlType)
{
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
		ComEvent.SetEvent();
	}

	return TRUE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
	exit(0);

	return ;
}

#endif


BOOL CGameService::Run()
{
	ComEvent.InitEvent(FALSE, FALSE);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerCloseEvent, TRUE);


#else
	if(SIG_ERR == signal(SIGINT, &HandlerCloseEvent))
	{
		CLog::GetInstancePtr()->AddLog("注册CTRL+C事件失败!");
	}

#endif

	ComEvent.Wait();

	return TRUE;
}


BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("新连接来到!");
	return TRUE;
}

BOOL CGameService::OnCloseConnect(CConnection *pConnection)
{
	CLog::GetInstancePtr()->AddLog("收到连接断开的事件!!!!!!");

	//以下是向各个系统投递连接断开的消息
	StDisConnectNotify DisConnectNotify;
	DisConnectNotify.u64ConnID = pConnection->GetConnectionID();
	DisConnectNotify.btConType = pConnection->GetConnectionType();
	IDataBuffer *pDataBuff = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(500);
	CBufferHelper WriteHelper(TRUE, pDataBuff);
	WriteHelper.BeginWrite(CMD_DISCONNECT_NOTIFY, 0,  0);
	WriteHelper.Write(DisConnectNotify);
	WriteHelper.EndWrite();

	IDataBuffer *pDataBuff2 = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pDataBuff->GetTotalLenth());
	if(pDataBuff2 == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	pDataBuff2->CopyFrom(pDataBuff);

	m_ServerCmdHandler.AddMessage(DisConnectNotify.u64ConnID, pDataBuff);
	m_ProxyCmdHandler.AddMessage(DisConnectNotify.u64ConnID, pDataBuff2);

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
	switch(pNetPacket->m_dwCmdID)
	{
	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();

	return TRUE;
}




BOOL CGameService::OnCmdGMCommand( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharGmCmdReq CharGmCmdReq;
	pBufferHelper->Read(CharGmCmdReq);


	return FALSE;
}

BOOL CGameService::OnCmdHeartBeatReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharHeartBeatReq CharHeartBeatReq;
	pBufferHelper->Read(CharHeartBeatReq);

	StCharHeartBeatAck CharHeartBeatAck;
	CharHeartBeatAck.dwReqTimestamp = CharHeartBeatReq.dwReqTimestamp;
	CharHeartBeatAck.dwServerTime   = CommonFunc::GetCurrTime();

	return TRUE;
}



BOOL CGameService::SendCmdToDBConnection(IDataBuffer *pDataBuf)
{
	if(m_dwDBConnID == 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	ASSERT(ServiceBase::GetInstancePtr()->SendCmdToConnection(m_dwDBConnID, pDataBuf));

	return TRUE;
}

BOOL CGameService::SendCmdToStatConnection(IDataBuffer *pDataBuf)
{
	if(m_dwStatConnID == 0)
	{
		return FALSE;
	}

	ASSERT(ServiceBase::GetInstancePtr()->SendCmdToConnection(m_dwStatConnID, pDataBuf));

	return TRUE;
}

