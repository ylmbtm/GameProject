#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "ConnectionType.h"
#include "PacketDef/TransferPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "StaticPlayerMgr.h"
#include "PacketDef/ClientPacket.h"

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

		return TRUE;
	}

	//这下面运行的函数都是多线程执行的函数，需要考虑重入的问题。
	switch(pBufferHelper->GetCommandHeader()->wCommandID)
	{
	case CMD_CHAR_ENTER_GAME_REQ:
		{
			CWillEnterNode *pWillEnterNode = m_WillEnterNodeMgr.GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
			if(pWillEnterNode == NULL)
			{
				//非法的进入
				break;
			}

			StCharEnterGameReq CharEnterGameReq;
			pBufferHelper->Read(CharEnterGameReq);
			 
			if(pWillEnterNode->m_dwIndentifyCode == CharEnterGameReq.dwIndentifyCode)
			{
				//非法的进入
				break;
			}

			CStaticPlayer *pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(CharEnterGameReq.u64CharID);
			if(pStaticPlayer == NULL)
			{
				pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->CreateStaicPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
				if(pStaticPlayer != NULL)
				{
					pStaticPlayer->SetGameSvrConnID(pWillEnterNode->m_GameSvrConnID);
					pStaticPlayer->SetSceneID(pWillEnterNode->m_dwSceneID);
				}
			}

			pBufferHelper->GetCommandHeader()->dwSceneID = pWillEnterNode->m_dwSceneID;

			RelayToServer(pStaticPlayer, pBufferHelper->GetDataBuffer());
		}
		break;
	case CMD_ROLE_MOVE:
		{
			CStaticPlayer *pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
			if(pStaticPlayer == NULL)
			{
				ASSERT_FAIELD;
			}

			RelayToServer(pStaticPlayer, pBufferHelper->GetDataBuffer());
		}
		break;
	case CMD_SVR_CHAR_WILL_ENTER:
		{
			StCharWillEnterGame CharWillEnterGame;
			pBufferHelper->Read(CharWillEnterGame);

			CWillEnterNode *pWillEnterNode = m_WillEnterNodeMgr.CreateWillEnterNode(CharWillEnterGame.u64CharID);
			pWillEnterNode->m_dwIndentifyCode = CharWillEnterGame.dwIdentifyCode;
			pWillEnterNode->m_GameSvrConnID   = CharWillEnterGame.dwGameSvrID;
			pWillEnterNode->m_dwSceneID		  = CharWillEnterGame.dwSceneID;

		}
		break;
	default:
		{
			CStaticPlayer *pClientObj = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
			if(pClientObj == NULL)
			{
				ASSERT_FAIELD;
			}

			if(u64ConnID < 1000)
			{
				RelayToClient(pClientObj, pBufferHelper->GetDataBuffer());
			}
			else
			{
				RelayToServer(pClientObj, pBufferHelper->GetDataBuffer());
			}
		}
	}

	
	return 0;
}

BOOL CGameService::StartRun()
{
	if(!CLog::GetInstancePtr()->StartLog("ProxyServer"))
	{
		return FALSE;
	}

	CLog::GetInstancePtr()->AddLog("---------服务器开始启动-----------");

	if(!CGlobalConfig::GetInstancePtr()->Load("ProxyServer.ini"))
	{
		CLog::GetInstancePtr()->AddLog("配制文件加载失败!");
		return FALSE;
	}

	if(!StartService())
	{
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
	g_GameService->StopService();

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

BOOL CGameService::RelayToServer( CStaticPlayer *pClientObj, IDataBuffer *pBuffer )
{
	if(pClientObj == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	SendCmdToConnection(pClientObj->GetGameSvrConnID(), pBuffer);

	return TRUE;
}

BOOL CGameService::RelayToClient( CStaticPlayer *pStaticPlayer, IDataBuffer *pBuffer )
{
	if(pStaticPlayer == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	SendCmdToConnection(pStaticPlayer->GetCharID(), pBuffer);

	return TRUE;
}

BOOL CGameService::OnDisconnect( CConnection *pConnection )
{
	CLog::GetInstancePtr()->AddLog("收到连接断开的事件!!!!!!");

	CStaticPlayer *pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(pConnection->GetConnectionID());
	if(pStaticPlayer == NULL)
	{
		CLog::GetInstancePtr()->AddLog("收到连接断开的事件， 但没有玩家连接数据!!!!!!");
		return TRUE;
	}

	StCharLeaveGameReq CharLeaveGameReq;

	CharLeaveGameReq.dwLeaveReason = 1;

	IDataBuffer *pSendBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(1024);
	if(pSendBuffer == NULL)
	{
		return FALSE;
	}

	CBufferHelper WriteHelper(TRUE, pSendBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_LEAVE_GAME_REQ, CMDH_SENCE, pStaticPlayer->GetSceneID(), pStaticPlayer->GetCharID());
	WriteHelper.Write(CharLeaveGameReq);
	WriteHelper.EndWrite();

	SendCmdToConnection(pStaticPlayer->GetGameSvrConnID(), pSendBuffer);

	pSendBuffer->Release();
	
	CStaticPlayerMgr::GetInstancePtr()->RemoveByCharID(pStaticPlayer->GetCharID());

	return TRUE;
}

/*

class ClientEngine
{
	BOOL InitEngine();

	BOOL CloseEngine();

	BOOL SetClientID(UINT64 u64ClientID);  //一般是角色ID

	BOOL SetLoginSvrInfo(char *szIpAddr, UINT16 sPort);

	BOOL SendData(char *pData, UINT32 dwLen);

	BOOL Login(char *pszAccountName, char *pszPassword);

	BOOL RegisterNetHandler(IMessageHandler *pMsgHandler);

	BOOL Render();
};

*/
