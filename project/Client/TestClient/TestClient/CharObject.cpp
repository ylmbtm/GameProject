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

UINT32 CCharObject::ReadFromBuffer( CBufferHelper *pBufHelper )
{
	UINT32 dwSize = 0;
	dwSize += CWorldObject::ReadFromBuffer(pBufHelper);

	return dwSize;
}
