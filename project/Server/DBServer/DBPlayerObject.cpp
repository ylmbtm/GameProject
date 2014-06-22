#include "stdafx.h"
#include "DBPlayerObject.h"
#include "DataBuffer/BufferHelper.h"

CDBPlayerObject::CDBPlayerObject()
{

}

CDBPlayerObject::~CDBPlayerObject()
{

}

UINT32 CDBPlayerObject::WriteToPacket( CBufferHelper *pWriteBuffer )
{
	UINT32 dwSize = 0;
	dwSize += WritePlayerBaseInfo(pWriteBuffer);

	return dwSize;
}

BOOL CDBPlayerObject::LoadFromDB()
{


	return TRUE;
}

UINT32 CDBPlayerObject::WritePlayerBaseInfo( CBufferHelper *pWriteBuffer )
{
	UINT32 dwSize = 0;
	dwSize += pWriteBuffer->Write(m_u64ObjectID);

	return dwSize;
}


