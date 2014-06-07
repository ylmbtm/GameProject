#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
#include "Scene.h"

typedef stdext::hash_map<UINT32, CScene*>  SceneMap;

class CSceneManager
{
public:
	CSceneManager();

	~CSceneManager();

	BOOL		CommandHandler(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	BOOL		CreateScene(UINT32 dwSceneID);

	CScene*		GetSceneByID(UINT32 dwSceneID);

	SceneMap	m_mapSceneList;
};

#endif //_SCENE_MANAGER_H_
