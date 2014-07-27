#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
#include "Scene.h"
#include "CmdHandler/CommonCmdHandler.h"

typedef stdext::hash_map<UINT32, CScene*>  SceneMap;

class CSceneManager : public CCommonCmdHandler
{
public:
	CSceneManager();

	~CSceneManager();

	BOOL		Init(UINT32 dwReserved);

	BOOL		Uninit();

	BOOL		OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL		OnUpdate( UINT32 dwTick );
	
public:
	BOOL		CreateScene(UINT32 dwSceneID);

	CScene*		GetSceneByID(UINT32 dwSceneID);


	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdCreateSceneReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);


	//*********************消息处理定义结束******************************

protected:
	SceneMap	m_mapSceneList;
};

#endif //_SCENE_MANAGER_H_
