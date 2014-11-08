#include "stdafx.h"
#include "BufferHelper.h"
#include "DataBuffer.h"

CBufferHelper::CBufferHelper(BOOL bWrite, IDataBuffer *pDataBuffer)
{
	m_bWriting = bWrite;

	m_pDataBuffer = pDataBuffer;
}

CBufferHelper::CBufferHelper(BOOL bWrite, UINT32 dwBuffSize)
{
	m_bWriting = bWrite;

	m_pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(dwBuffSize);
}

CBufferHelper::~CBufferHelper(void)
{
}

BOOL CBufferHelper::BeginWrite(UINT16 wCommandID, UINT8 CmdHandleID, UINT16 dwSceneID, UINT64 u64CharID)
{
	if(m_pDataBuffer == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	GetTransferHeader()->CheckCode = 0xff;
	CommandHeader *pCommandHeader = GetCommandHeader();

	pCommandHeader->wCommandID = wCommandID;
	pCommandHeader->dwSceneID  = dwSceneID;
	pCommandHeader->CmdHandleID= CmdHandleID;
	pCommandHeader->u64CharID  = u64CharID;

	m_dwCurPos	  = sizeof(TransferHeader) + sizeof(CommandHeader);

	m_pDataBuffer->SetDataLenth(m_dwCurPos);

	return TRUE;
}

BOOL CBufferHelper::EndWrite()
{
	GetTransferHeader()->dwSize = m_dwCurPos;

	m_pDataBuffer->SetDataLenth(m_dwCurPos);

	return TRUE;
}


BOOL CBufferHelper::BeginRead()
{
	if(m_pDataBuffer == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_dwCurPos	  = sizeof(TransferHeader) + sizeof(CommandHeader);

	m_pDataBuffer->SetDataLenth(m_pDataBuffer->GetDataLenth());

	if(GetTransferHeader()->CheckCode != 0xff)
	{
		ASSERT_FAIELD;
		return FALSE;

	}

	return TRUE;
}

BOOL CBufferHelper::IsWriting()
{
	return m_bWriting;
}

CommandHeader* CBufferHelper::GetCommandHeader()
{
	return (CommandHeader*)m_pDataBuffer->GetBufferPos(sizeof(TransferHeader));
}

TransferHeader* CBufferHelper::GetTransferHeader()
{
	return (TransferHeader*)m_pDataBuffer->GetBuffer();
}

UINT32 CBufferHelper::Read( CHAR *pszValue )
{
	ASSERT(pszValue != NULL);

	UINT16 wLen = 0;

	Read(wLen);

	memcpy(pszValue, m_pDataBuffer->GetData() + m_dwCurPos, wLen);

	pszValue[wLen] = 0;

	m_dwCurPos += wLen;

	return wLen + sizeof(UINT16);
}

UINT32 CBufferHelper::Read( BYTE *pData, UINT32 dwBytes )
{
	ASSERT(pData != NULL);

	memcpy(pData, m_pDataBuffer->GetData() + m_dwCurPos, dwBytes);

	m_dwCurPos += dwBytes;

	return dwBytes;
}

UINT32 CBufferHelper::Read( std::string &strValue )
{
	CHAR  szValue[1024];

	UINT32 dwSize = Read(szValue);

	strValue = szValue;

	return dwSize;
}

BOOL CBufferHelper::Seek( INT32 nOffset )
{
	m_dwCurPos += nOffset;

	return TRUE;
}

UINT32 CBufferHelper::Write( BYTE *pData, UINT32 dwBytes )
{
	ASSERT(pData != NULL);

	memcpy(m_pDataBuffer->GetData() + m_dwCurPos, pData,  dwBytes);

	m_dwCurPos += dwBytes;

	return dwBytes;
}

UINT32 CBufferHelper::Write( const CHAR *pszValue )
{
	ASSERT(pszValue != NULL);

	UINT16 wLen = strlen(pszValue);

	Write(wLen);

	memcpy(m_pDataBuffer->GetData() + m_dwCurPos, pszValue,  wLen);

	m_dwCurPos += wLen;

	return wLen + sizeof(UINT16);
}

UINT32 CBufferHelper::Write( std::string &strValue )
{
	return Write(strValue.c_str());
}

UINT32 CBufferHelper::Write( CHAR *pszValue )
{
	UINT16 wLen = strlen(pszValue)+1;

	Write(wLen);

	memcpy(m_pDataBuffer->GetData() + m_dwCurPos, pszValue,  wLen);

	m_dwCurPos += wLen;

	return wLen + sizeof(UINT16);
}

IDataBuffer* CBufferHelper::GetDataBuffer()
{
	return m_pDataBuffer;
}

UINT8* CBufferHelper::GetCurrentPoint()
{
	return (UINT8*)(m_pDataBuffer->GetData() + m_dwCurPos);
}

UINT32 CBufferHelper::WriteCheckBufferCode()
{
	UINT32 dwCheckCode = 0x11111111;
	Write(dwCheckCode);

	return sizeof(dwCheckCode);
}

UINT32 CBufferHelper::ReadCheckBufferCode()
{
	UINT32 dwCheckCode = 0;
	Read(dwCheckCode);
	if(dwCheckCode != 0x11111111)
	{
		ASSERT_FAIELD;
	}

	return sizeof(dwCheckCode);
}



