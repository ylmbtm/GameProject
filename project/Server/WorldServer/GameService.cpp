#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/ClientPacket.h"
#include "ConnectionType.h"
#include "DataBuffer/BufferHelper.h"

CGameService::CGameService(void)
{

}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
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

	m_ServerCmdHandler.AddMessage(DisConnectNotify.u64ConnID, pDataBuff);

	//pDataBuff->Release();

	return TRUE;
}

BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
	//if(pBufferHelper->GetPacketHeader()->CmdHandleID == CMDH_SVR_CON)
	//{
	//	m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	//
	//	return TRUE;
	//}

	//m_WorldCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());


	switch(pNetPacket->m_dwCmdID)
	{
	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("WorldServer", "log"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(!CConfigFile::GetInstancePtr()->Load("WorldServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("world_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("world_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");

		return FALSE;
	}

	UINT32 nStatPort = CConfigFile::GetInstancePtr()->GetIntValue("stat_svr_port");
	std::string strStatIp = CConfigFile::GetInstancePtr()->GetStringValue("stat_svr_ip");
	ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strStatIp, nStatPort);


	UINT32 nDBPort = CConfigFile::GetInstancePtr()->GetIntValue("stat_svr_port");
	std::string strDBIp = CConfigFile::GetInstancePtr()->GetStringValue("db_svr_ip");
	ServiceBase::GetInstancePtr()->ConnectToOtherSvr(strDBIp, nDBPort);

	if(!m_ServerCmdHandler.Init(0))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动默认连接消息处理器失败!");
		return FALSE;
	}

	m_WorldCmdHandler.Init(0);

	return TRUE;
}


BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();

	return TRUE;
}


CCommonEvent ComEvent;

#ifdef WIN32
BOOL WINAPI HandlerCloseEvent(DWORD dwCtrlType)
{
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
        ServiceBase::GetInstancePtr()->StopNetwork();

		ComEvent.SetEvent();
	}

	return FALSE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
    CGameService::GetInstancePtr()->StopNetwork();

	ComEvent.SetEvent();

	return ;
}

#endif


BOOL CGameService::Run()
{
    while(TRUE)
    {
        char sz[100];
        gets(sz);

        if(strcmp(sz,"exit") == 0)
        {
            break;
        }
    }

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
