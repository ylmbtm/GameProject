#include "StdAfx.h"
#include "StaticPlayerMgr.h"

CStaticPlayer::CStaticPlayer()
{
	m_GameSvrConnID = 0;
}

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
	iterator itor = find(u64CharID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}

CStaticPlayer* CStaticPlayerMgr::CreateStaicPlayer(UINT64 u64CharID)
{
	CStaticPlayer *pClientObj = new CStaticPlayer;

	pClientObj->m_u64CharID = u64CharID;

	insert(std::make_pair(u64CharID, pClientObj));

	return pClientObj;
}

BOOL CStaticPlayerMgr::RemoveByCharID(UINT64 u64CharID)
{
	erase(u64CharID);

	return TRUE;
}


