#ifndef __CHAR_WILL_ENTER_LISTH__
#define __CHAR_WILL_ENTER_LISTH__
#include "Utility/AVLTree.h"

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

class CWillEnterNodeMgr : public AVLTree<UINT64, CWillEnterNode>
{
public:
	CWillEnterNodeMgr(void);
	~CWillEnterNodeMgr(void);

	CWillEnterNode* GetByCharID(UINT64 u64CharID);

	CWillEnterNode* CreateWillEnterNode(UINT64 u64CharID);

	BOOL		    RemoveByCharID(UINT64 u64CharID);

	void			DoEnumNode(TNodeTypePtr pNode)
	{
		printf("%d", pNode->m_Data);
	}
};

#endif /*__CHAR_WILL_ENTER_LISTH__*/