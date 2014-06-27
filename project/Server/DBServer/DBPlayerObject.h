#ifndef __DB_PLAYER_OBJECT_H__
#define __DB_PLAYER_OBJECT_H__
#include "Utility/AVLTree.h"
#include "GameDefine.h"
#include "Utility/Position.h"

class CDBPlayerObject
{
public:
	CDBPlayerObject();

	~CDBPlayerObject();
public:
	UINT32	WriteToPacket( CBufferHelper *pWriteBuffer );

	UINT32  WritePlayerBaseInfo( CBufferHelper *pWriteBuffer );

	BOOL	LoadFromDB();
public:
	UINT64		m_u64ObjectID;
	CHAR		m_szObjectName[MAX_NAME_LEN];
	CPosition	m_ObjectPos;
	UINT32		m_dwFeature;
	UINT32      m_dwLevel;
	


};



class CDBPlayerObjectMgr : public AVLTree<UINT64, CDBPlayerObject>
{
public:
	CDBPlayerObjectMgr()
	{

	}

	~CDBPlayerObjectMgr()
	{

	}



public:
	CDBPlayerObject*   GetPlayer(UINT64 u64CharID)
	{
		return GetByKey(u64CharID);
	}

	CDBPlayerObject*   CreatePlayerByID(UINT64 u64CharID)
	{
		return InsertAlloc(u64CharID);
	}

public:
};

#endif //__DB_PLAYER_OBJECT_H__
