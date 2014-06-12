#include "StdAfx.h"
#include "CharWillEnterList.h"

CWillEnterNode::CWillEnterNode()
{
	m_GameSvrConnID = 0;
}

CWillEnterNode::~CWillEnterNode()
{

}

UINT64 CWillEnterNode::GetGameSvrConnID()
{
	return m_GameSvrConnID;
}

UINT64 CWillEnterNode::GetCharID()
{
	return m_u64CharID;
}


CWillEnterNodeMgr::CWillEnterNodeMgr(void)
{
}

CWillEnterNodeMgr::~CWillEnterNodeMgr(void)
{
}

CWillEnterNodeMgr* CWillEnterNodeMgr::GetInstancePtr()
{
	static CWillEnterNodeMgr _WillEnterNodeMgr;

	return &_WillEnterNodeMgr;
}

CWillEnterNode* CWillEnterNodeMgr::GetByCharID(UINT64 u64CharID)
{
	iterator itor = find(u64CharID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}

CWillEnterNode* CWillEnterNodeMgr::CreateWillEnterNode(UINT64 u64CharID)
{
	CWillEnterNode *pWillEnterNode = new CWillEnterNode;

	pWillEnterNode->m_u64CharID = u64CharID;

	insert(std::make_pair(u64CharID, pWillEnterNode));

	return pWillEnterNode;
}

BOOL CWillEnterNodeMgr::RemoveByCharID(UINT64 u64CharID)
{
	erase(u64CharID);

	return TRUE;
}


