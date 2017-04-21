#ifndef _BUFFER_HELPER_H_
#define _BUFFER_HELPER_H_

#include "PacketDef/PacketHeader.h"

#include "IBufferHandler.h"

class CBufferHelper
{
public:
	CBufferHelper(BOOL bWrite, IDataBuffer *pDataBuffer);
	CBufferHelper(BOOL bWrite, UINT32 dwBuffSize);
	~CBufferHelper(void);

public:
	BOOL		BeginWrite(UINT16 wCommandID, UINT16 dwSceneID, UINT64 u64CharID);
	
	BOOL		BeginRead();

	BOOL		EndWrite();

	BOOL		IsWriting();

	PacketHeader* GetPacketHeader();

	IDataBuffer*  GetDataBuffer();

	UINT8*	      GetCurrentPoint();

	template <typename T>
	UINT32		Write(T& _Value);

	UINT32		Write(std::string &strValue);

	UINT32		Write(CHAR *pszValue);

	UINT32		Write(const CHAR *pszValue);

	UINT32		Write(BYTE *pData, UINT32 dwBytes);

	template<typename T>
	UINT32		Read(T &_value);

	UINT32		Read(std::string &strValue);

	UINT32		Read(CHAR *pszValue);

	UINT32		Read(BYTE *pData, UINT32 dwBytes);

	BOOL		Seek(INT32 nOffset);

	UINT32		WriteCheckBufferCode();

	UINT32		ReadCheckBufferCode();

private:
	IDataBuffer *m_pDataBuffer;

	UINT32 m_dwCurPos;

	BOOL	m_bWriting;
};

template<typename T>
UINT32 CBufferHelper::Read( T &_value )
{
	if((m_dwCurPos+sizeof(T)) > m_pDataBuffer->GetTotalLenth())
	{
		ASSERT_FAIELD;

		return 0;
	}

	_value = *(T*)(m_pDataBuffer->GetBuffer() + m_dwCurPos);

	m_dwCurPos   += sizeof(T);

	return sizeof(T);
}


template <typename T>
UINT32 CBufferHelper::Write( T& _Value )
{
	T *pValue = (T*)(m_pDataBuffer->GetBuffer()+m_dwCurPos);

	*pValue = _Value;

	m_dwCurPos += sizeof(T);

	ASSERT((m_dwCurPos+100) <= m_pDataBuffer->GetBufferSize());

	return sizeof(T);
}

#endif
