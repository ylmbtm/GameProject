#ifndef __DB_PLAYER_OBJECT_H__
#define __DB_PLAYER_OBJECT_H__
#include "Utility/AVLTree.h"

class CDBPlayerObject
{
public:
	CDBPlayerObject();

	~CDBPlayerObject();
public:
	UINT32	WriteToPacket(CBufferHelper &WriteBuffer);

	BOOL	LoadFromDB();

public:
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
