#ifndef __OBJECT_H__
#define __OBJECT_H__

class CObject
{
public:
	CObject();

	~CObject();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual BOOL    SetUpdate(UpdateTypeEnum UpdateType);

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);
public:



	UINT8			GetObjectType();

	UINT64&			GetObjectID();

	VOID			SetObjectID(UINT64 u64ObjectID);

protected:
	UINT64			m_u64ObjectID;	
};

#endif //__OBJECT_H__
