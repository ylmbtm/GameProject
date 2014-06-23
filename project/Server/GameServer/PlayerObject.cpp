#include "stdafx.h"
#include "PlayerObject.h"
#include "DataBuffer\BufferHelper.h"

CPlayerObject::CPlayerObject()
{

}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
	case 1:
		{

		}
		break;
	default:
		{

		}
		break;
	}

	return TRUE;
}

UINT64 CPlayerObject::GetConnectID()
{
	return m_u64ConnID;
}

VOID CPlayerObject::SetConnectID( UINT64 u64ConnID )
{
	m_u64ConnID = u64ConnID;

	return ;
}

BOOL CPlayerObject::IsChanged()
{

	return TRUE;
}

BOOL CPlayerObject::ClearChangeFlag()
{

	return TRUE;
}

UINT32 CPlayerObject::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::WriteToBuffer(pBufHelper, dwChangeFlag, dwDest);




	return dwSize;
}

UINT32 CPlayerObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::ReadFromBuffer(pBufHelper);


	return dwSize;
}

BOOL CPlayerObject::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}

BOOL CPlayerObject::LoadFromDBPcket( CBufferHelper *pBufferHelper )
{
	pBufferHelper->Read(m_u64ObjectID);
	pBufferHelper->Read(m_szObjectName);
	pBufferHelper->Read(m_ObjectPos);

	return TRUE;
}

BOOL CPlayerObjectMgr::AddPlayer( CPlayerObject *pObject )
{
	insert(std::make_pair(pObject->GetObjectID(), pObject));

	return TRUE;
}

CPlayerObject* CPlayerObjectMgr::GetPlayer( UINT64 ObjectID )
{
	iterator itor  = find(ObjectID);
	if(itor != end())
	{
		return itor->second;
	}

	return NULL;
}



CWorldObject* CUpdateObjectMgr::GetFisrtOjbect()
{
	iterator itor = begin();
	if(itor == end())
	{
		return NULL;
	}

	CWorldObject *pWorldObject = *itor;

	erase(itor);

	return pWorldObject;
}
