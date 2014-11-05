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
	
	//  初始化玩家对象
	BOOL	Init();
	//  反初始化玩家对家
	BOOL	Uninit();
	
	//从数据库包中读取对像数据
	UINT32	LoadFromDBPacket(CBufferHelper *pBufferHelper);

	UINT32  SaveDataToPacket(CBufferHelper *pBufferHelper);

public:
	UINT64		m_u64ObjectID;
	CHAR		m_szObjectName[MAX_NAME_LEN];
	UINT32		m_dwSceneID;
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
