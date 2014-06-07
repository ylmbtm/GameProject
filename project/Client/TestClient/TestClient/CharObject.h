#ifndef _CHAR_OBJECT_H_
#define _CHAR_OBJECT_H_
#include "WorldObject.h"

class CCharObject : public CWorldObject
{
public:
	CCharObject();

	~CCharObject();

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);
};

#endif //_CHAR_OBJECT_H_
