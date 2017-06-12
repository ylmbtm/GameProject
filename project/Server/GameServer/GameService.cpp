#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"

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


BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("GameServer", "log"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("GameServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("game_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("game_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn,this))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");
		return FALSE;
	}

	if(!m_ServerCmdHandler.Init(0))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动默认连接消息处理器失败!");
		return FALSE;
	}

	if(!m_SceneManager.Init(0))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动场景管理器失败!");
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

	return FALSE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
    CGameService::GetInstancePtr()->StopNetwork();

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



BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
// 	if(pBufferHelper->GetPacketHeader()->CmdHandleID == CMDH_SVR_CON)
// 	{
// 		m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
// 	}
// 	else
// 	{
// 		m_SceneManager.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
// 	}

	return TRUE;
}

BOOL CGameService::OnCloseConnect( CConnection *pConnection )
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

	m_SceneManager.AddMessage(DisConnectNotify.u64ConnID, pDataBuff2);

	//pDataBuff->Release();

	return TRUE;
}

BOOL CGameService::Uninit()
{
	ServiceBase::GetInstancePtr()->StopNetwork();

	return TRUE;
}

BOOL CGameService::OnNewConnect(CConnection *pConn)
{
	CLog::GetInstancePtr()->AddLog("新连接来到!");
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
