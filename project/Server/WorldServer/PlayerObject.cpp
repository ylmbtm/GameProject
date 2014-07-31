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

BOOL CPlayerObject::LoadFromDB()
{
	strncpy(m_szObjectName, "zhangming", MAX_NAME_LEN);
	m_ObjectPos.x = 100;
	m_ObjectPos.y = 100;
	m_ObjectPos.z = 100;

	return TRUE;
}


BOOL CPlayerObject::LoadFromDBPcket( CBufferHelper *pBufferHelper )
{
	return TRUE;
}


CPlayerObject* CPlayerObjectMgr::GetPlayer( UINT64 u64CharID )
{
	return GetByKey(u64CharID);
}

CPlayerObject* CPlayerObjectMgr::CreatePlayerByID( UINT64 u64CharID )
{
	return InsertAlloc(u64CharID);
}

BOOL CPlayerObjectMgr::ReleasePlayer( UINT32 u64CharID )
{
	return Delete(u64CharID);
}
