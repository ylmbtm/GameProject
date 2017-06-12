#include "stdafx.h"
#include "ObjectID.h"
#include "ConnectionType.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/ClientPacket.h"
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


BOOL CGameService::DispatchPacket(NetPacket *pNetPacket)
{
// 	if(pBufferHelper->GetPacketHeader()->CmdHandleID == CMDH_SVR_CON)
// 	{
// 		m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
// 	}
// 	else
// 	{
// 		//如果需要开启很多的线程时， 按账号划分玩家的线程 
// 		m_DBCmdHandler[0].AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
// 	}

	return TRUE;
}

BOOL CGameService::Init()
{
	CommonFunc::SetCurrentWorkPath("");

	if(!CLog::GetInstancePtr()->StartLog("DBServer", "log"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("DBServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	UINT16 nPort = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_port");
	INT32  nMaxConn = CConfigFile::GetInstancePtr()->GetIntValue("db_svr_max_con");
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

	for(int i = 0; i < DB_THREAD_NUM; i++)
	{
		m_DBCmdHandler[i].Init(0);
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

BOOL CGameService::OnCloseConnect( CConnection *pConnection )
{
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
