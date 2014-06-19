#ifndef __CLIENT_OBJECT_MGR_H__
#define __CLIENT_OBJECT_MGR_H__
#include "Utility\AVLTree.h"

enum LineState
{
	LS_Online,
	Ls_OffLine,
	Ls_Unknow
};


class  CStaticPlayer
{
public:
	CStaticPlayer():m_u64CharID(0),m_dwSceneID(0),m_GameSvrConnID(0){}

	~CStaticPlayer();

public:
	UINT64	GetGameSvrConnID();

	VOID	SetGameSvrConnID(UINT64 u64ConnID);

	UINT64	GetCharID();

	VOID    SetSceneID(UINT32 dwSceneID);

	UINT32  GetSceneID();

public:
	UINT64	m_u64CharID;

	UINT32	m_dwSceneID;

	UINT64	m_GameSvrConnID;
};

class CStaticPlayerMgr : public AVLTree<UINT64, CStaticPlayer>
{
private:
	CStaticPlayerMgr(void);
	~CStaticPlayerMgr(void);

public:
	static CStaticPlayerMgr* GetInstancePtr();

	CStaticPlayer* GetByCharID(UINT64 u64CharID);

	CStaticPlayer* CreateStaicPlayer(UINT64 u64CharID);

	BOOL		   RemoveByCharID(UINT64 u64CharID);

	BOOL		   GetPlayerOnline(UINT64 u64CharID);
};

#endif /*__CLIENT_OBJECT_MGR_H__*/