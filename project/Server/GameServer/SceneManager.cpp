#include "stdafx.h"
#include "SceneManager.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketHeader.h"
#include "DataBuffer/BufferHelper.h"



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

BOOL CSceneManager::CreateScene( UINT32 dwSceneID )
{
	CScene *pScene = new CScene;

	if(!pScene->Init(dwSceneID))
	{
		delete pScene;
		return FALSE;
	}

	m_mapSceneList.insert(std::make_pair(dwSceneID, pScene));

	return TRUE;
}

BOOL CSceneManager::CommandHandler(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	CommandHeader *pHeader = pBufferHelper->GetCommandHeader();
	if(pHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CScene *pScene = GetSceneByID(pHeader->dwSceneID);
	if(pScene != NULL)
	{
		pScene->AddMessage(u64ConnID, pBufferHelper->GetDataBuffer());
	}
		
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
