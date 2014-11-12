#include "stdafx.h"
#include "PlayerObject.h"
#include "DataBuffer/BufferHelper.h"

CPlayerObject::CPlayerObject()
{
	
}

CPlayerObject::~CPlayerObject()
{

}

BOOL CPlayerObject::Init()
{
	memset(m_szObjectName, 0, MAX_NAME_LEN);
	m_u64ObjectID = 0;

	m_ObjectPos.x = 0;
	m_ObjectPos.y = 0;
	m_ObjectPos.z = 0;

	m_dwFeature = 0;
	m_dwLevel   = 0;

	

	return TRUE;
}

BOOL CPlayerObject::Uninit()
{

	return TRUE;
}


UINT32 CPlayerObject::LoadFromDBPacket( CBufferHelper *pBufferHelper )
{
	UINT32 dwSize = 0;

	dwSize += pBufferHelper->Read(m_u64ObjectID);
	dwSize += pBufferHelper->Read(m_szObjectName);
	dwSize += pBufferHelper->Read(m_dwSceneID);
	dwSize += pBufferHelper->Read(m_dwFeature);
	dwSize += pBufferHelper->Read(m_ObjectPos.x);
	dwSize += pBufferHelper->Read(m_ObjectPos.y);
	dwSize += pBufferHelper->Read(m_ObjectPos.z);

	return dwSize;
}

UINT32 CPlayerObject::SaveDataToPacket( CBufferHelper *pBufferHelper )
{
	UINT32 dwSize = 0;

	dwSize += pBufferHelper->Write(m_u64ObjectID);
	dwSize += pBufferHelper->Write(m_szObjectName);
	dwSize += pBufferHelper->Write(m_dwFeature);
	dwSize += pBufferHelper->Write(m_ObjectPos.x);
	dwSize += pBufferHelper->Write(m_ObjectPos.y);
	dwSize += pBufferHelper->Write(m_ObjectPos.z);

	return dwSize;
}

UINT64 CPlayerObject::GetObjectID()
{
	return m_u64ObjectID;
}


CPlayerObject* CPlayerObjectMgr::GetPlayer( UINT64 u64CharID )
{
	return GetByKey(u64CharID);
}

CPlayerObject* CPlayerObjectMgr::CreatePlayerByID( UINT64 u64CharID )
{
	return InsertAlloc(u64CharID);
}

BOOL CPlayerObjectMgr::ReleasePlayer( UINT64 u64CharID )
{
	return Delete(u64CharID);
}
