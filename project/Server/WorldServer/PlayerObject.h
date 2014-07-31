#ifndef __WS_PLAYER_OBJECT_H__
#define __WS_PLAYER_OBJECT_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"
#include "Utility/Position.h"

class CPlayerObject
{
public:
	CPlayerObject();

	~CPlayerObject();
	
	BOOL	Init();

	BOOL	Uninit();
	
	BOOL	LoadFromDB();

	BOOL	LoadFromDBPcket(CBufferHelper *pBufferHelper);

public:
	UINT64		m_u64ObjectID;
	CHAR		m_szObjectName[MAX_NAME_LEN];
	CPosition	m_ObjectPos;
	UINT32		m_dwFeature;
	UINT32      m_dwLevel;
};


class CPlayerObjectMgr : public AVLTree<UINT64, CPlayerObject>
{
public:
	CPlayerObjectMgr()
	{
	}

	~CPlayerObjectMgr()
	{
	}



public:
	CPlayerObject*		GetPlayer(UINT64 u64CharID);

	CPlayerObject*		CreatePlayerByID(UINT64 u64CharID);

	BOOL			    ReleasePlayer(UINT32 u64CharID);

public:
};

#endif //__WS_PLAYER_OBJECT_H__
