#include "stdafx.h"
#include "CharObject.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/BufferHelper.h"



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

	dwSize += pBufHelper->Read(m_szObjectName);

	return dwSize;
}
