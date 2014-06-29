#ifndef _CHAR_OBJECT_H_
#define _CHAR_OBJECT_H_
#include "WorldObject.h"
#include "GameDefine.h"

class CCharObject : public CWorldObject
{
public:
	CCharObject();

	~CCharObject();

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

	char		m_szObjectName[MAX_NAME_LEN];
};

#endif //_CHAR_OBJECT_H_
