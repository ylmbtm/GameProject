#include "stdafx.h"
#include "CharWillEnterList.h"

CWillEnterNode::CWillEnterNode()
{
}

CWillEnterNode::~CWillEnterNode()
{

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

CWillEnterNode* CWillEnterNodeMgr::GetByCharID(UINT64 u64CharID)
{
	return GetByKey(u64CharID);
}

CWillEnterNode* CWillEnterNodeMgr::CreateWillEnterNode(UINT64 u64CharID)
{
	return InsertAlloc(u64CharID);
}

BOOL CWillEnterNodeMgr::RemoveByCharID(UINT64 u64CharID)
{
	return Delete(u64CharID);;
}

void CWillEnterNodeMgr::DoEnumNode( TNodeTypePtr pNode )
{
	printf("-->%lld", pNode->m_Key);
}


