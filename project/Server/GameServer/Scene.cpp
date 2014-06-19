#include "stdafx.h"
#include "CommandDef.h"
#include "GameDef.h"
#include "PacketHeader.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"

#include "PacketDef/ClientPacket.h"
#include "PacketDef/CommonPacket.h"

#include "DataBuffer/DataBuffer.h"
#include "DataBuffer/BufferHelper.h"
#include "GameService.h"
#include "Scene.h"



CScene::CScene()
{
	
}

CScene::~CScene()
{

}

BOOL CScene::Init(UINT32 dwSceneID)
{
	m_dwSceneID = dwSceneID;
	
	m_WorkThread.SetCommandHandler(this);

	m_WorkThread.Start();

	m_GridManager.Init(-1000, 1000, -1000, 1000);

	return TRUE;
}

BOOL CScene::Uninit()
{
	m_WorkThread.Stop();

	return TRUE;
}

BOOL CScene::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	CommandHeader *pCmdHeader = pBufferHelper->GetCommandHeader();
	if(pCmdHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	switch(wCommandID)
	{
	PROCESS_COMMAND_ITEM(CMD_CHAR_LOGIN_REQ,		OnCmdRoleLogin);
	PROCESS_COMMAND_ITEM(CMD_CHAR_ENTER_GAME_REQ,	OnCmdEnterGameReq);
	PROCESS_COMMAND_ITEM(CMD_CHAR_LEAVE_GAME_REQ,	OnCmdLeaveGameReq);
	PROCESS_COMMAND_ITEM(CMD_ROLE_MOVE,				OnCmdPlayerMove);
	PROCESS_COMMAND_ITEM(CMD_DB_GET_CHAR_ACK,		OnCmdDBGetCharAck);
	default:
		{
			CPlayerObject *pPlayerObject = m_PlayerObjectMgr.GetPlayer(pCmdHeader->u64CharID);
			if(pPlayerObject != NULL)
			{
				pPlayerObject->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper);
			}
		}
		break;
	}

	return TRUE;
}

BOOL CScene::AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer)
{
	return m_WorkThread.AddMessage(u64ConnID, pDataBuffer);
}

BOOL CScene::AddToMap( CWorldObject *pWorldObject)
{
	INT32 dwIndex = m_GridManager.GetIndexByPos(pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);
	if(dwIndex == -1)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CGrid *pGrid = m_GridManager.GetGridByIndex(dwIndex);
	if(pGrid == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	ASSERT(!pGrid->IsObjectExist(pWorldObject));

	pGrid->AddObject(pWorldObject);

	pWorldObject->SetOwnerScene(this);

	INT32 Grids[10];

	m_GridManager.GetSurroundingGrids(dwIndex, Grids);

	SendCreateObjectToGrids(pWorldObject, Grids);

	if(pWorldObject->GetObjectType() == OBJECT_PLAYER)
	{
		SendCreateGridsToPlayer(Grids, (CPlayerObject*)pWorldObject);
	}

	return TRUE;
}

INT32 CScene::OnCmdRoleLogin( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	
	return 0;
}

INT32 CScene::OnCmdEnterGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameReq CharEnterGameReq;

	pBufferHelper->Read(CharEnterGameReq);

	CPlayerObject *pPlayerObject = new CPlayerObject;

	pPlayerObject->SetObjectID(CharEnterGameReq.u64CharID);

	pPlayerObject->SetPosition(70, 0, 80);

	pPlayerObject->SetConnectID(u64ConnID);

	m_PlayerObjectMgr.AddPlayer(pPlayerObject);

	m_UpdateObjectMgr.AddUpdateObject(pPlayerObject);

	IDataBuffer *pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(1024);
	if(pDataBuffer != NULL)
	{
		StCharEnterGameAck CharEnterGameAck;
		CharEnterGameAck.dwIndentifyCode = CharEnterGameReq.dwIndentifyCode;
		CharEnterGameAck.dwSceneID       = GetSceneID();

		CBufferHelper WriteHelper(TRUE, pDataBuffer);

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_ACK, CMDH_SENCE, 0,  CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameAck);

		pPlayerObject->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_DEST_MYSELF);

		WriteHelper.EndWrite();

		CGameService::GetInstancePtr()->SendCmdToConnection(u64ConnID, pDataBuffer);
	}

	AddToMap(pPlayerObject);

	return 0;
}

INT32 CScene::OnCmdPlayerMove( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharMoveReq CharMoveReq;
	pBufferHelper->Read(CharMoveReq);

	CPlayerObject *pPlayerObj = m_PlayerObjectMgr.GetPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
	if(pPlayerObj == NULL)
	{
		ASSERT_FAIELD;
		return 0;
	}

	INT32 nDestIndex = m_GridManager.GetIndexByPos(CharMoveReq.x, CharMoveReq.z);
	if(nDestIndex == -1)
	{
		//不合法的地址
		ASSERT_FAIELD;
		return 0;
	}


	INT32 nSrcIndex  = m_GridManager.GetIndexByPos(pPlayerObj->m_ObjectPos.x, pPlayerObj->m_ObjectPos.z);

	ASSERT(m_GridManager.IsObjectExist(pPlayerObj, nSrcIndex));

	if(nDestIndex == nSrcIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
	{
		pPlayerObj->m_ObjectPos.x = CharMoveReq.x;
		pPlayerObj->m_ObjectPos.y = CharMoveReq.y;
		pPlayerObj->m_ObjectPos.z = CharMoveReq.z;

		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nDestIndex, Grids);

		SendUpdatePlayerToGrids(pPlayerObj, Grids);

		return 0;
	}

	ASSERT(!m_GridManager.IsObjectExist(pPlayerObj, nDestIndex));

	m_GridManager.RemoveObjectFromGrid(pPlayerObj, nSrcIndex);

	pPlayerObj->m_ObjectPos.x = CharMoveReq.x;
	pPlayerObj->m_ObjectPos.y = CharMoveReq.y;
	pPlayerObj->m_ObjectPos.z = CharMoveReq.z;

	m_GridManager.AddObjectToGrid(pPlayerObj, nDestIndex);

	INT32 AddGrid[10], RemoveGrid[10], StayGrid[10];

	m_GridManager.CalDiffGrids(nSrcIndex, nDestIndex, AddGrid, RemoveGrid, StayGrid);

	//将玩家发的更新发给周围的格子里
	SendUpdatePlayerToGrids(pPlayerObj, StayGrid);

	SendCreateObjectToGrids(pPlayerObj, AddGrid);

	SendCreateGridsToPlayer(AddGrid, pPlayerObj);

	SendRemovePlayerToGrids(pPlayerObj->GetObjectID(), RemoveGrid);

	SendRemoveGridsToPlayer(RemoveGrid, pPlayerObj);

	return 0;
}

BOOL CScene::SendCreateObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9])
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, 0, 0, 0);
	UINT8 objType = pWorldObject->GetObjectType();
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(objType);
	pWorldObject->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_DEST_OTHER);
	WriteHelper.EndWrite();
	//变化包组装完成
	
	UINT32 i = 0;
	while(Grids[i] != -1)
	{	
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;
		//生成完整其它玩家包, 发送当前玩家
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			if(pIterObj->GetObjectID() != pWorldObject->GetObjectID())
			{
				if(pIterObj->GetObjectType() == OBJECT_PLAYER)
				{
					CLog::GetInstancePtr()->AddLog("自己【%d】--Add--To--%d, 坐标 x = %f, z=%f", (UINT32)pWorldObject->GetObjectID(),
						(UINT32)pIterObj->GetObjectID(), pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);

					CGameService::GetInstancePtr()->SendCmdToConnection(((CPlayerObject*)pIterObj)->GetConnectID(),pIterObj->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendCreateGridsToPlayer( INT32 Grids[9], CPlayerObject *pPlayerObj )
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, 0, 0, pPlayerObj->GetObjectID());

	UINT32 *pCount = (UINT32*)WriteHelper.GetCurrentPoint();
	*pCount = 0;
	WriteHelper.Write(*pCount);

	UINT32 i = 0;
	while(Grids[i] != -1)
	{	
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CWorldObject *pIterObj = pGrid->m_pHead;
		//生成完整其它玩家包，发送当前玩家
		while(pIterObj != NULL)
		{
			if(pIterObj->GetObjectID() != pPlayerObj->GetObjectID())
			{
				UINT8 objType = pIterObj->GetObjectType();

				WriteHelper.Write(objType);

				pIterObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_DEST_OTHER);

				CLog::GetInstancePtr()->AddLog("自己【%d】--Add--From---%d, 坐标 x = %f, z=%f", 
					(UINT32)pIterObj->GetObjectID(), (UINT32)pPlayerObj->GetObjectID(), pIterObj->m_ObjectPos.x, pIterObj->m_ObjectPos.z);

				(*pCount)++;
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	WriteHelper.EndWrite();

	if((*pCount) <= 0)
	{
		return TRUE;
	}

	CGameService::GetInstancePtr()->SendCmdToConnection(pPlayerObj->GetConnectID(), pPlayerObj->GetObjectID(), 0,  &m_WriteBuffer);

	return TRUE;
}

BOOL CScene::SendUpdatePlayerToGrids( CPlayerObject *pPlayerObj, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_UPDATE, 0, 0, 0);
	UINT8 objType = pPlayerObj->GetObjectType();
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(pPlayerObj->GetObjectID());
	pPlayerObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_DEST_OTHER);
	WriteHelper.EndWrite();
	//变化包组装完成

	int i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;

		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			if(pIterObj->GetObjectID() != pPlayerObj->GetObjectID())
			{
				if(pIterObj->GetObjectType() == OBJECT_PLAYER)
				{
					CPlayerObject *pPlayer = (CPlayerObject*)pIterObj;
					CGameService::GetInstancePtr()->SendCmdToConnection(pPlayer->GetConnectID(), pPlayer->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendRemovePlayerToGrids( UINT64 u64CharID, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE, 0, 0, 0);
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(u64CharID);
	WriteHelper.EndWrite();
	//变化包组装完成

	UINT32 i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CWorldObject *pIterObj = pGrid->m_pHead;
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			CPlayerObject *pPlayer = (CPlayerObject*)pIterObj;

			if(pPlayer->GetObjectID() == u64CharID)
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己【%d】--Remove--From---%d, ",  (UINT32)u64CharID, (UINT32)pIterObj->GetObjectID());

			CGameService::GetInstancePtr()->SendCmdToConnection(pPlayer->GetConnectID(), pPlayer->GetObjectID(), 0, &m_WriteBuffer);

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}


	return TRUE;
}

BOOL CScene::SendRemoveGridsToPlayer( INT32 Grids[9], CPlayerObject *pPlayerObj)
{
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE, 0, 0, 0);
	
	UINT32 *pCount = (UINT32*)WriteHelper.GetCurrentPoint();
	UINT32 Value = 0;
	WriteHelper.Write(Value);

	UINT32 i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			WriteHelper.Write(pIterObj->GetObjectID());

			if(pIterObj->GetObjectID() == pPlayerObj->GetObjectID())
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己【%d】--Remove--From---%d, ",  (UINT32)pPlayerObj->GetObjectID(), (UINT32)pIterObj->GetObjectID());

			*pCount = *pCount + 1;

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	WriteHelper.EndWrite();

	if((*pCount) <= 0)
	{
		return TRUE;
	}

	CGameService::GetInstancePtr()->SendCmdToConnection(pPlayerObj->GetConnectID(), pPlayerObj->GetObjectID(), 0, &m_WriteBuffer);

	return TRUE;
}

INT32 CScene::OnCmdLeaveGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLeaveGameReq CharLeaveGameReq;

	pBufferHelper->Read(CharLeaveGameReq);

	//if(CharLeaveGameReq.dwLeaveReason == 1)

	CPlayerObject *pPlayerObject = m_PlayerObjectMgr.GetPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
	if(pPlayerObject == NULL)
	{
		ASSERT_FAIELD;
		return 0;
	}

	RemoveFromMap(pPlayerObject);

	m_PlayerObjectMgr.erase(pPlayerObject->GetObjectID());

	delete pPlayerObject;

	return 0;
}

BOOL CScene::RemoveFromMap( CWorldObject *pWorldObject )
{
	INT32 dwIndex = m_GridManager.GetIndexByPos(pWorldObject->GetPosition().x, pWorldObject->GetPosition().z);
	if(dwIndex == -1)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CGrid *pGrid = m_GridManager.GetGridByIndex(dwIndex);
	if(pGrid == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	pGrid->RemoveObject(pWorldObject);

	pWorldObject->SetOwnerScene(NULL);

	INT32 Grids[10];
	m_GridManager.GetSurroundingGrids(dwIndex, Grids);

	SendRemovePlayerToGrids(pWorldObject->GetObjectID(), Grids);

	return TRUE;

}

BOOL CScene::OnUpdate( UINT32 dwTick )
{
	CWorldObject *pWorldObject = m_UpdateObjectMgr.GetFisrtOjbect();
	if(pWorldObject == NULL)
	{
		return TRUE;
	}

	return TRUE;
}

BOOL CScene::AddToUpdateList( CWorldObject *pWorldObject )
{
	m_UpdateObjectMgr.AddUpdateObject(pWorldObject);

	return TRUE;
}

INT32 CScene::OnCmdDBGetCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{

	return 0;
}

