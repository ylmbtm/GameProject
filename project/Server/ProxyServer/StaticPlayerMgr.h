#ifndef __CLIENT_OBJECT_MGR_H__
#define __CLIENT_OBJECT_MGR_H__

enum LineState
{
	LS_Online,
	Ls_OffLine,
	Ls_Unknow
};


class  CStaticPlayer
{
public:
	CStaticPlayer();

	~CStaticPlayer();

public:
	UINT64	GetGameSvrConnID();

	VOID	SetGameSvrConnID(UINT64 u64ConnID);

	UINT64	GetCharID();

public:
	UINT64 m_u64CharID;

	UINT64 m_GameSvrConnID;
};

class CStaticPlayerMgr : public stdext::hash_map<UINT64, CStaticPlayer*>
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


	//stdext::hash_map<UINT64, CStaticPlayer*> m_vtOnlinePlayer;
	//stdext::hash_map<UINT64, CStaticPlayer*> m_vtOfflinePlayer;
};

#endif /*__CLIENT_OBJECT_MGR_H__*/