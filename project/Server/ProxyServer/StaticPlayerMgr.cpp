#include "stdafx.h"
#include "StaticPlayerMgr.h"

CStaticPlayer::~CStaticPlayer()
{

}

UINT64 CStaticPlayer::GetGameSvrConnID()
{
	return m_GameSvrConnID;
}

UINT64 CStaticPlayer::GetCharID()
{
	return m_u64CharID;
}

VOID CStaticPlayer::SetGameSvrConnID( UINT64 u64ConnID )
{
	m_GameSvrConnID = u64ConnID;
}

VOID CStaticPlayer::SetSceneID( UINT32 dwSceneID )
{
	m_dwSceneID = dwSceneID;
}

UINT32 CStaticPlayer::GetSceneID()
{
	return m_dwSceneID;
}

CStaticPlayerMgr::CStaticPlayerMgr(void)
{
}

CStaticPlayerMgr::~CStaticPlayerMgr(void)
{
}

CStaticPlayerMgr* CStaticPlayerMgr::GetInstancePtr()
{
	static CStaticPlayerMgr _StaicPlayerMgr;

	return &_StaicPlayerMgr;
}

CStaticPlayer* CStaticPlayerMgr::GetByCharID(UINT64 u64CharID)
{
	return GetByKey(u64CharID);
}

CStaticPlayer* CStaticPlayerMgr::CreateStaicPlayer(UINT64 u64CharID)
{
	CStaticPlayer *pClientObj = InsertAlloc(u64CharID);
	if(pClientObj == NULL)
	{
		return NULL;
	}

	pClientObj->m_u64CharID = u64CharID;

	return pClientObj;
}

BOOL CStaticPlayerMgr::RemoveByCharID(UINT64 u64CharID)
{
	return Delete(u64CharID);
}


