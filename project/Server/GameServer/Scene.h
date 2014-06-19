#ifndef _SCENE_H_
#define _SCENE_H_
#include "CmdHandler/CommonWorkThread.h"
#include "Playerobject.h"
#include "WorldObject.h"
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

	BOOL AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL AddToMap(CWorldObject *pWorldObject);

	BOOL RemoveFromMap(CWorldObject *pWorldObject);

	BOOL AddToUpdateList(CWorldObject *pWorldObject);


public:
	UINT32	GetSceneID(){ return m_dwSceneID; }

public:
	UINT32							m_dwSceneID;



protected:
	CCommonWorkThread				m_WorkThread;			//工作线程

	CDataBuffer<CONST_BUFF_SIZE>	m_WriteBuffer;			//固定的写buffer

	CPlayerObjectMgr				m_PlayerObjectMgr;		//玩家管理器

	CGridManager					m_GridManager;			//地图格子管理器

	CUpdateObjectMgr				m_UpdateObjectMgr;		//更新的对象管理器
	
	//*********************消息处理定义开始******************************
public:
	INT32 OnCmdEnterGameReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	INT32 OnCmdLeaveGameReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	INT32 OnCmdPlayerMove(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	INT32 OnCmdDBGetCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	//*********************消息处理定义结束******************************

	BOOL SendCreateObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9]);

	BOOL SendCreateGridsToPlayer(INT32 Grids[9], CPlayerObject *pPlayerObj);

	BOOL SendUpdatePlayerToGrids(CPlayerObject *pPlayerObj, INT32 Grids[9]);

	BOOL SendRemovePlayerToGrids(UINT64 u64CharID, INT32 Grids[9]);

	BOOL SendRemoveGridsToPlayer(INT32 Grids[9], CPlayerObject *pPlayerObj);


	
};

#endif //_MAP_H_


