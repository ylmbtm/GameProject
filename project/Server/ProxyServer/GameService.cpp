#include "stdafx.h"
#include "GameService.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "StaticPlayerMgr.h"
#include "PacketDef/ClientPacket.h"
#include "ObjectID.h"

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
	//从底层传过来的内存，在这个地方是不释放的, 由处理者去释放
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
			CHECK_PAYER_ID(pBufferHelper->GetCommandHeader()->u64CharID);
			CWillEnterNode *pWillEnterNode = m_WillEnterNodeMgr.GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
			if(pWillEnterNode == NULL)
			{
				//非法的进入
				ASSERT_FAIELD;
				break;
			}

			StCharEnterGameReq CharEnterGameReq;
			pBufferHelper->Read(CharEnterGameReq);

			CHECK_PAYER_ID(CharEnterGameReq.u64CharID);
			 
			if(pWillEnterNode->m_dwIdentifyCode != CharEnterGameReq.dwIdentifyCode)
			{
				m_WillEnterNodeMgr.RemoveByCharID(CharEnterGameReq.u64CharID);
				//非法的进入
				break;
			}

			CStaticPlayer *pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(CharEnterGameReq.u64CharID);
			if(pStaticPlayer == NULL)
			{
				pStaticPlayer = CStaticPlayerMgr::GetInstancePtr()->CreateStaicPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
				if(pStaticPlayer != NULL)
				{
					/*pStaticPlayer->SetGameSvrConnID(pWillEnterNode->m_GameSvrConnID);
					pStaticPlayer->SetSceneID(pWillEnterNode->m_dwSceneID);*/
				}
				else
				{
					ASSERT_FAIELD;
					break;
				}
			}

			m_WillEnterNodeMgr.RemoveByCharID(CharEnterGameReq.u64CharID);

			pBufferHelper->GetCommandHeader()->dwSceneID = pWillEnterNode->m_dwSceneID;

			//发向世界服

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

			pBufferHelper->GetCommandHeader()->dwSceneID = pStaticPlayer->GetSceneID();

			RelayToServer(pStaticPlayer, pBufferHelper->GetDataBuffer());
		}
		break;
	case CMD_SVR_CHAR_WILL_ENTER:
		{
			StCharWillEnterGame CharWillEnterGame;
			pBufferHelper->Read(CharWillEnterGame);

			CWillEnterNode *pWillEnterNode = m_WillEnterNodeMgr.CreateWillEnterNode(CharWillEnterGame.u64CharID);
			if(pWillEnterNode == NULL)
			{
				ASSERT_FAIELD;
				break;
			}

			pWillEnterNode->m_dwIdentifyCode  = CharWillEnterGame.dwIdentifyCode;
		}
		break;
	case CMD_CHAR_GAME_MANAGER:
		{
			CConnection *pConn = GetConnectionByID(u64ConnID);
			if(pConn == NULL)
			{
				break;
			}

			//判断IP是否合法
			if(pConn->m_dwIpAddr == 0)
			{

			}

			StCharGmCmdReq CharGmCmdReq;
			pBufferHelper->Read(CharGmCmdReq);
	
			if(!OnCmdGMCommand(CMD_CHAR_GAME_MANAGER, u64ConnID, pBufferHelper))
			{
				CStaticPlayer *pClientObj = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
				if(pClientObj == NULL)
				{
					ASSERT_FAIELD;
					break;
				}

				RelayToServer(pClientObj, pBufferHelper->GetDataBuffer());
			}
		}
		break;
	default:
		{
			CStaticPlayer *pClientObj = CStaticPlayerMgr::GetInstancePtr()->GetByCharID(pBufferHelper->GetCommandHeader()->u64CharID);
			if(pClientObj == NULL)
			{
				ASSERT_FAIELD;
				break;
			}

			if(u64ConnID < 1000)
			{
				RelayToClient(pClientObj, pBufferHelper->GetDataBuffer());
			}
			else
			{
				pBufferHelper->GetCommandHeader()->dwSceneID = pClientObj->GetSceneID();

				RelayToServer(pClientObj, pBufferHelper->GetDataBuffer());
			}
		}
	}

	pBufferHelper->GetDataBuffer()->Release();

	
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

	if(!StartService())
	{
		CLog::GetInstancePtr()->AddLog("启动服务失败!");
		ASSERT_FAIELD;
		return FALSE;
	}

	if(!m_ServerCmdHandler.Init(0))
	{
		ASSERT_FAIELD;
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
		CGameService::GetInstancePtr()->StopService();

		ComEvent.SetEvent();
	}

	return TRUE;
}
#else
void  HandlerCloseEvent(int nSignalNum)
{
	CGameService::GetInstancePtr()->StopService();

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

	if(!SendCmdToConnection(pClientObj->GetGameSvrConnID(), pBuffer))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}

BOOL CGameService::RelayToClient( CStaticPlayer *pStaticPlayer, IDataBuffer *pBuffer )
{
	if(pStaticPlayer == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	if(!SendCmdToConnection(pStaticPlayer->GetCharID(), pBuffer))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

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
	CharHeartBeatAck.dwServerTime   = CommonFunc::GetTime();

	return TRUE;
}

