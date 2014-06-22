#include "stdafx.h"
#include "CharObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/BufferHelper.h"



CCharObject::CCharObject()
{
	memset(m_szObjectName, 0 , MAX_NAME_LEN);
}

CCharObject::~CCharObject()
{

}

BOOL CCharObject::IsChanged()
{

	return TRUE;
}

BOOL CCharObject::ClearChangeFlag()
{

	return TRUE;
}

UINT32 CCharObject::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{
	UINT32 dwSize = 0;
	dwSize += CWorldObject::WriteToBuffer(pBufHelper, dwChangeFlag, dwDest);

	dwSize += pBufHelper->Write(m_szObjectName);


	return dwSize;
}

UINT32 CCharObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += CWorldObject::ReadFromBuffer(pBufHelper);

	dwSize += pBufHelper->Read(m_szObjectName);

	return dwSize;
}

char* CCharObject::GetObjectName()
{
	return m_szObjectName;
}

void CCharObject::SetObjectName( char *szName )
{
	if(szName == NULL)
	{
		ASSERT_FAIELD;
		return ;
	}

	strncpy(m_szObjectName, szName, MAX_NAME_LEN);
}
