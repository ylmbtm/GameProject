#include "stdafx.h"
#include "CharObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"



CCharObject::CCharObject()
{

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


	return dwSize;
}

UINT32 CCharObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += CWorldObject::ReadFromBuffer(pBufHelper);

	return dwSize;
}
