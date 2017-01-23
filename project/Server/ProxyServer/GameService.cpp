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
	m_dwWorldServerID = 0;
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
	//从底层传过来的内存，在这个地方是不释放的, 由处理者去释放
	if(pBufferHelper->GetPacketHeader()->CmdHandleID == CMDH_SVR_CON)
	{
		m_ServerCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}
	else
	{
		m_ProxyCmdHandler.AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}

	return TRUE;
}

BOOL CGameService::StartRun()
{
	if(!CLog::GetInstancePtr()->StartLog("ProxyServer"))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CGlobalConfig::GetInstancePtr()->Load("ProxyServer.ini"))
	{
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		ASSERT_FAIELD;
		return FALSE;
	}

    if(!SetMaxConnection(6000))
    {
        ASSERT_FAIELD;
        CLog::GetInstancePtr()->AddLog("设置服务器的最大连接数!");
        return FALSE;
    }

    if(!StartNetwork())
    {
        CLog::GetInstancePtr()->AddLog("启动服务失败!");
        ASSERT_FAIELD;
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

	OnIdle();

	return TRUE;
}

CCommonEvent ComEvent;

#ifdef WIN32
BOOL WINAPI HandlerCloseEvent(DWORD dwCtrlType)
{
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
        CGameService::GetInstancePtr()->StopNetwork();

		ComEvent.SetEvent();
	}

	return TRUE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
    CGameService::GetInstancePtr()->StopNetwork();

	exit(0);

	return ;
}

#endif


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

	ComEvent.Wait();

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

	IDataBuffer *pDataBuff2 = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pDataBuff->GetDataLenth());
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

BOOL CGameService::SetWorldServerID( UINT32 dwSvrID )
{
	m_dwWorldServerID = dwSvrID;

	return TRUE;
}

