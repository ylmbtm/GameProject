#include "stdafx.h"
#include "Object.h"
#include "DataBuffer\BufferHelper.h"


CObject::CObject()
{
	m_u64ObjectID = 0;
}

CObject::~CObject()
{

}

UINT8 CObject::GetObjectType()
{
	return (UINT8)(m_u64ObjectID>>56);
}

UINT64& CObject::GetObjectID()
{
	return m_u64ObjectID;
}

VOID CObject::SetObjectID( UINT64 u64ObjectID )
{
	m_u64ObjectID = u64ObjectID;

	return ;
}

BOOL CObject::IsChanged()
{
	return TRUE;
}

BOOL CObject::ClearChangeFlag()
{

	return TRUE;
}

BOOL CObject::SetUpdate(UpdateStatusEnum UpdateStatus)
{

	return TRUE;
}


UINT32 CObject::WriteToBuffer(CBufferHelper *pBufHelper,  UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;
	dwSize += pBufHelper->Write(m_u64ObjectID);

	return dwSize;
}

UINT32 CObject::ReadFromBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag)
{
	UINT32 dwSize = 0;
	dwSize += pBufHelper->Read(m_u64ObjectID);

	return dwSize;
}

