#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/PacketHeader.h"
#include "DataBuffer/BufferHelper.h"
#include "PacketDef/ServerPacket.h"
#include "GameService.h"
#include "Error.h"



CSceneManager::CSceneManager()
{

}

CSceneManager::~CSceneManager()
{
	SceneMap::iterator itor = m_mapSceneList.begin();
	for(; itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;
		if(pScene != NULL)
		{
			pScene->Uninit();
			delete pScene;
			pScene = NULL;
		}
	}
}

BOOL CSceneManager::Init(UINT32 dwReserved)
{
	if(!LoadDefaultScene())
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CCommonCmdHandler::Init(dwReserved);

	return TRUE;
}

BOOL CSceneManager::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CSceneManager::CreateScene( UINT32 dwSceneID )
{
	CScene *pScene = new CScene;

	if(!pScene->Init(dwSceneID, -1000, 1000, -1000, 1000))
	{
		ASSERT_FAIELD;

		delete pScene;

		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwSceneID, pScene));

	return TRUE;
}

BOOL CSceneManager::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	BOOL bHandled = TRUE;

	switch(wCommandID)
	{
	case CMD_SVR_CREATE_SCENE_REQ:
		{
			OnCmdCreateSceneReq(wCommandID, u64ConnID, pBufferHelper);
		}
		break;
	case CMD_DISCONNECT_NOTIFY:
		{

		}
		break;
	default:
		{
			bHandled = FALSE;
		}
		break;
	}

	if(bHandled) //消息己经被处理
	{
		return TRUE;
	}

	PacketHeader *pPacketHeader = pBufferHelper->GetPacketHeader();
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CScene *pScene = GetSceneByID(pPacketHeader->dwSceneID);
	if(pScene == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pScene->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper);
		
	return TRUE;
}

CScene* CSceneManager::GetSceneByID( UINT32 dwSceneID )
{
	SceneMap::iterator itor = m_mapSceneList.find(dwSceneID);
	if(itor != m_mapSceneList.end())
	{
		return itor->second;
	}

	return NULL;
}

BOOL CSceneManager::OnUpdate( UINT32 dwTick )
{
	for(SceneMap::iterator itor = m_mapSceneList.begin(); itor != m_mapSceneList.end(); ++itor)
	{
		CScene *pScene = itor->second;

		pScene->OnUpdate(dwTick);
	}

	return TRUE;
}

BOOL CSceneManager::OnCmdCreateSceneReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StSvrCreateSceneReq CreateSceneReq;
	pBufferHelper->Read(CreateSceneReq);

	StSvrCreateSceneAck CreateSceneAck;
	CreateSceneAck.dwCreateParam = CreateSceneReq.CreateParam;
	CreateSceneAck.dwSceneID = CreateSceneReq.dwSceneID;
	CreateSceneAck.dwServerID= CGameService::GetInstancePtr()->GetServerID();

	if (!CreateScene(CreateSceneReq.dwSceneID))
	{
		ASSERT_FAIELD;

		CreateSceneAck.dwAckCode = E_FAILED;
	}
	else
	{
		CreateSceneAck.dwAckCode = E_SUCCESSED;
	}
	
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_SVR_CREATE_SCENE_ACK, 0, 0);
	WriteHelper.Write(CreateSceneAck);
	WriteHelper.EndWrite();

	ServiceBase::GetInstancePtr()->SendCmdToConnection(u64ConnID, &m_WriteBuffer);

	return TRUE;
}

BOOL CSceneManager::LoadDefaultScene()
{
	if(!CreateScene(12))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	/*StSvrCreateSceneAck CreateSceneAck;
	CreateSceneAck.dwCreateParam = 0;
	CreateSceneAck.dwSceneID = 12;
	CreateSceneAck.dwServerID= CGameService::GetInstancePtr()->GetServerID();

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_SVR_CREATE_SCENE_ACK, CMDH_OTHER, 0, 0);
	WriteHelper.Write(CreateSceneAck);
	WriteHelper.EndWrite();

	ServiceBase::GetInstancePtr()->SendCmdToConnection(CGameService::GetInstancePtr()->m_dwWorldServerID, &m_WriteBuffer);*/

	return TRUE;
}
