#ifndef __CHAR_WILL_ENTER_LISTH__
#define __CHAR_WILL_ENTER_LISTH__

class  CWillEnterNode
{
public:
	CWillEnterNode();

	~CWillEnterNode();

public:
	UINT64	GetGameSvrConnID();

	UINT64	GetCharID();

public:
	UINT64 m_u64CharID;

	UINT64 m_GameSvrConnID;

	UINT32 m_dwIndentifyCode;

	UINT32 m_dwTickount;
};

class CWillEnterNodeMgr : public stdext::hash_map<UINT64, CWillEnterNode*>
{
private:
	CWillEnterNodeMgr(void);
	~CWillEnterNodeMgr(void);

public:
	static CWillEnterNodeMgr* GetInstancePtr();

	CWillEnterNode* GetByCharID(UINT64 u64CharID);

	CWillEnterNode* CreateWillEnterNode(UINT64 u64CharID);

	BOOL		   RemoveByCharID(UINT64 u64CharID);
};

#endif /*__CHAR_WILL_ENTER_LISTH__*/