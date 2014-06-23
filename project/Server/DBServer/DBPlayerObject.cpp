#include "stdafx.h"
#include "DBPlayerObject.h"
#include "DataBuffer/BufferHelper.h"

CDBPlayerObject::CDBPlayerObject()
{
	memset(m_szObjectName, 0, MAX_NAME_LEN);
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
	strncpy(m_szObjectName, "zhangming", MAX_NAME_LEN);
	m_ObjectPos.x = 100;
	m_ObjectPos.y = 100;
	m_ObjectPos.z = 100;


	return TRUE;
}

UINT32 CDBPlayerObject::WritePlayerBaseInfo( CBufferHelper *pWriteBuffer )
{
	UINT32 dwSize = 0;
	dwSize += pWriteBuffer->Write(m_u64ObjectID);
	dwSize += pWriteBuffer->Write(m_szObjectName);
	dwSize += pWriteBuffer->Write(m_ObjectPos);

	return dwSize;
}


