#ifndef _CHAR_OBJECT_H_
#define _CHAR_OBJECT_H_
#include "WorldObject.h"
#include "GameDef.h"

class CCharObject : public CWorldObject
{
public:
	CCharObject();

	~CCharObject();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

	char*			GetObjectName();

	void			SetObjectName(char *szName);

protected:
	char			m_szObjectName[MAX_NAME_LEN];
};

#endif //_CHAR_OBJECT_H_
