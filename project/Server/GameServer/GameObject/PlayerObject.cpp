#include "stdafx.h"
#include "PlayerObject.h"
#include "DataBuffer/BufferHelper.h"

CPlayerObject::CPlayerObject()
{
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
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

UINT32 CPlayerObject::GetConnectID()
{
	return m_dwConnID;
}

VOID CPlayerObject::SetConnectID( UINT32 dwConnID )
{
	m_dwConnID = dwConnID;

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
	
	if(dwChangeFlag == UPDATE_FLAG_CHANGE)
	{
		
	}
	else if(dwChangeFlag == UPDATE_FLAG_CREATE)
	{
		dwSize += pBufHelper->Write(m_dwFeature);
	}

	return dwSize;
}

UINT32 CPlayerObject::ReadFromBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag)
{
	UINT32 dwSize = 0;
	dwSize += CCharObject::ReadFromBuffer(pBufHelper, dwChangeFlag);

	if(dwChangeFlag == UPDATE_FLAG_CHANGE)
	{

	}
	else if(dwChangeFlag == UPDATE_FLAG_CREATE)
	{
		dwSize += pBufHelper->Read(m_dwFeature);
	}


	return dwSize;
}

BOOL CPlayerObject::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}

UINT32 CPlayerObject::LoadFromDBPacket( CBufferHelper *pBufferHelper )
{
	UINT32 dwSize = 0;

	dwSize += pBufferHelper->Read(m_u64ObjectID);
	dwSize += pBufferHelper->Read(m_szObjectName);
	dwSize += pBufferHelper->Read(m_dwFeature);
	dwSize += pBufferHelper->Read(m_ObjectPos.x);
	dwSize += pBufferHelper->Read(m_ObjectPos.y);
	dwSize += pBufferHelper->Read(m_ObjectPos.z);

	return dwSize;
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

BOOL CPlayerObjectMgr::RemovePlayer( UINT64 ObjectID )
{
	if(GetPlayer(ObjectID) == NULL)
	{
		ASSERT_FAIELD;
	}

	erase(ObjectID);

	return TRUE;
}

BOOL CUpdateObjectMgr::AddUpdateObject( CWorldObject *pObject )
{
	insert(pObject);

	return TRUE;
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

BOOL CUpdateObjectMgr::RemoveUpdateObject( CWorldObject *pObject )
{
	iterator itor = find(pObject);
	if(itor == end())
	{
		return FALSE;
	}

	erase(itor);

	return TRUE;
}


