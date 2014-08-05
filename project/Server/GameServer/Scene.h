#ifndef _SCENE_H_
#define _SCENE_H_
#include "CmdHandler/CommonWorkThread.h"
#include "GameObject/Playerobject.h"
#include "GameObject/WorldObject.h"
#include "GridManager.h"
#include "DataBuffer/DataBuffer.h"


class CScene : public ICommandHandler
{
public:
	CScene();

	~CScene();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick);

	BOOL Init(UINT32 dwSceneID);

	BOOL Uninit();

	BOOL AddToMap(CWorldObject *pWorldObject);

	BOOL RemoveFromMap(CWorldObject *pWorldObject);

	BOOL AddToUpdateList(CWorldObject *pWorldObject);

	BOOL RemoveFromUpList(CWorldObject *pWorldObject);

	BOOL HandleUpdateList();

	BOOL HandleUpdateObject(CWorldObject *pWorldObject);


public:
	UINT32	GetSceneID(){ return m_dwSceneID; }

protected:
	UINT32							m_dwSceneID;

	CDataBuffer<CONST_BUFF_SIZE>	m_WriteBuffer;			//固定的写buffer

	CPlayerObjectMgr				m_PlayerObjectMgr;		//玩家管理器

	CGridManager					m_GridManager;			//地图格子管理器

	CUpdateObjectMgr				m_UpdateObjectMgr;		//更新的对象管理器
	
	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdLeaveGameReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdPlayerMove(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDBLoadCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	//*********************消息处理定义结束******************************

	BOOL SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9]);

	BOOL SendNewGridsToObject(INT32 Grids[9], CPlayerObject *pPlayerObj);

	BOOL SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9]);

	BOOL SendRemoveObjectToGrids(UINT64 u64CharID, INT32 Grids[9]);

	BOOL SendRemoveGridsToPlayer(INT32 Grids[9], CPlayerObject *pPlayerObj);

	BOOL SendUpdateObjectToMyself(CWorldObject *pWorldObj);


	
};

#endif //_MAP_H_


