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


BOOL CGameService::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	if(pBufferHelper->GetCommandHeader()->CmdHandleID == CMDH_SVR_CON)
	{
		m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}

	return TRUE;
}

BOOL CGameService::OnDisconnect( CConnection *pConnection )
{
	CLog::GetInstancePtr()->AddLog("收到连接断开的事件!!!!!!");

	//以下是向各个系统投递连接断开的消息
	StDisConnectNotify DisConnectNotify;
	DisConnectNotify.u64ConnID = pConnection->GetConnectionID();
	DisConnectNotify.btConType = pConnection->GetConnectionType();
	IDataBuffer *pDataBuff = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(500);
	CBufferHelper WriteHelper(TRUE, pDataBuff);
	WriteHelper.BeginWrite(CMD_DISCONNECT_NOTIFY, CMDH_SVR_CON, 0,  0);
	WriteHelper.Write(DisConnectNotify);
	WriteHelper.EndWrite();

	m_ServerCmdHandler.AddMessage(DisConnectNotify.u64ConnID, pDataBuff);

	pDataBuff->Release();

	return TRUE;
}


BOOL CGameService::StartRun()
{
	if(!CLog::GetInstancePtr()->StartLog("StatisticsServer"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	if(!CGlobalConfig::GetInstancePtr()->Load("StatisticsServer.ini"))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	if(!StartService())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("启动服务失败!");

		return FALSE;
	}

	m_ServerCmdHandler.Init(0);

	OnIdle();

	return TRUE;
}



CCommonEvent ComEvent;

#ifdef WIN32
BOOL WINAPI HandlerCloseEvent(DWORD dwCtrlType)
{
	if(dwCtrlType == CTRL_CLOSE_EVENT)
	{
		CGameService::GetInstancePtr()->StopService();

		ComEvent.SetEvent();
	}

	return FALSE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
	CGameService::GetInstancePtr()->StopService();

	ComEvent.SetEvent();

	return ;
}

#endif

/*
BOOL CGameService::OnIdle()
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

	while(TRUE)
	{
		char sz[100];
		gets_s(sz, 100);

		if(strcmp(sz,"exit") == 0)
		{
			MessageBox(NULL, "ok", "ok", MB_OK);
		}
	}


	ComEvent.Wait();

	return TRUE;
}
*/

BOOL CGameService::OnIdle()
{
	while(TRUE)
	{
		char sz[100];
		gets_s(sz, 100);

		if(strcmp(sz,"exit") == 0)
		{
			CGameService::GetInstancePtr()->StopService();
			break;
		}
	}

	return TRUE;
}

