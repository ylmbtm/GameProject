#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_
#include "Utility/Position.h"
#include "Object.h"
class CGrid;
class CScene;

template <typename TContainer, typename TElem>
class CNodeLink
{
public:
	CNodeLink()
	{
		m_pNext = NULL;
		m_pPrev = NULL;
		m_pGrid = NULL;
	}

	TElem		*m_pNext;
	TElem		*m_pPrev;
	TContainer	*m_pContainer;
};

class CWorldObject : public CObject
{
public:
	CWorldObject();

	~CWorldObject();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual BOOL    SetUpdate();

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);

public:
	CPosition&	GetPosition();

	VOID		SetPosition(FLOAT x, FLOAT y, FLOAT z);

	CScene*     GetOwnerScene();

	VOID		SetOwnerScene(CScene *pScene);

public:
	//CNodeLink<CGrid, CWorldObject>	m_GridLink;

	CWorldObject		*m_pGridNext;
	CWorldObject		*m_pGridPrev;

public:

	CPosition	m_ObjectPos;

	CScene		*m_pOwnerScene;		

};

#endif //_WORLD_OBJECT_H_
