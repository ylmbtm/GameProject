#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "ObjectID.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketDef/PacketHeader.h"
#include "PacketDef/ClientPacket.h"
#include "PacketDef/CommonPacket.h"
#include "PacketDef/DBPacket.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/DataBuffer.h"
#include "DataBuffer/BufferHelper.h"






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
	return m_WorkThread.Stop();
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
	PROCESS_COMMAND_ITEM(CMD_CHAR_ENTER_GAME_REQ,	OnCmdEnterGameReq);
	PROCESS_COMMAND_ITEM(CMD_CHAR_LEAVE_GAME_REQ,	OnCmdLeaveGameReq);
	PROCESS_COMMAND_ITEM(CMD_DB_LOAD_CHAR_ACK,		OnCmdDBLoadCharAck);
	PROCESS_COMMAND_ITEM(CMD_ROLE_MOVE,				OnCmdPlayerMove);
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

BOOL CScene::AddToMap(CWorldObject *pWorldObject)
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

	pWorldObject->SetUpdate(UT_New);

	return TRUE;
}


INT32 CScene::OnCmdEnterGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameReq CharEnterGameReq;
	pBufferHelper->Read(CharEnterGameReq);

	CHECK_PAYER_ID(CharEnterGameReq.u64CharID);

	StDBLoadCharInfoReq DBLoadCharInfoReq;
	DBLoadCharInfoReq.dwSceneID = m_dwSceneID;
	DBLoadCharInfoReq.dwGameSvrID= CGameService::GetInstancePtr()->GetServerID();
	DBLoadCharInfoReq.u64CharID = CharEnterGameReq.u64CharID;
	DBLoadCharInfoReq.dwProxySvrID = u64ConnID;
	
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_LOAD_CHAR_REQ, CMDH_OTHER, 0, 0);
	WriteHelper.Write(DBLoadCharInfoReq);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);


	return TRUE;
}

INT32 CScene::OnCmdPlayerMove( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharMoveReq CharMoveReq;
	pBufferHelper->Read(CharMoveReq);

	CPlayerObject *pPlayerObj = m_PlayerObjectMgr.GetPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
	if(pPlayerObj == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nDestIndex = m_GridManager.GetIndexByPos(CharMoveReq.x, CharMoveReq.z);
	if(nDestIndex == -1)
	{
		//不合法的目标地址
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nCurIndex  = m_GridManager.GetIndexByPos(pPlayerObj->m_ObjectPos.x, pPlayerObj->m_ObjectPos.z);

	ASSERT(m_GridManager.IsObjectExist(pPlayerObj, nCurIndex));

	if(nDestIndex != nCurIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
	{
		ASSERT(!m_GridManager.IsObjectExist(pPlayerObj, nDestIndex));

		m_GridManager.RemoveObjectFromGrid(pPlayerObj, nCurIndex);

		m_GridManager.AddObjectToGrid(pPlayerObj, nDestIndex);
	}
	
	pPlayerObj->m_ObjectPos.x = CharMoveReq.x;
	pPlayerObj->m_ObjectPos.y = CharMoveReq.y;
	pPlayerObj->m_ObjectPos.z = CharMoveReq.z;

	pPlayerObj->SetUpdate(UT_Update);

	return TRUE;
}

BOOL CScene::SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9])
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, CMDH_OTHER, 0, 0);
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
			ASSERT_FAIELD;
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
					CLog::GetInstancePtr()->AddLog("自己[%lld]--Add--To--[%lld], 坐标 x = %f, z=%f", pWorldObject->GetObjectID(),
						pIterObj->GetObjectID(), pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);

					CGameService::GetInstancePtr()->SendCmdToConnection(((CPlayerObject*)pIterObj)->GetConnectID(),pIterObj->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendNewGridsToObject( INT32 Grids[9], CPlayerObject *pPlayerObj )
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, CMDH_OTHER, 0, pPlayerObj->GetObjectID());

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

				CLog::GetInstancePtr()->AddLog("自己[%lld]--Add--From---[%lld], 坐标 x = %f, z=%f", 
					pIterObj->GetObjectID(), pPlayerObj->GetObjectID(), pIterObj->m_ObjectPos.x, pIterObj->m_ObjectPos.z);

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

BOOL CScene::SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_UPDATE, CMDH_OTHER, 0, 0);
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(pWorldObj->GetObjectID());
	pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_DEST_OTHER);
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
			if(pIterObj->GetObjectID() != pWorldObj->GetObjectID())
			{
				if(pIterObj->GetObjectType() == OBJECT_PLAYER)
				{
					CPlayerObject *pIterPlayer = (CPlayerObject*)pIterObj;
					CGameService::GetInstancePtr()->SendCmdToConnection(pIterPlayer->GetConnectID(), pIterPlayer->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendRemoveObjectToGrids( UINT64 u64CharID, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE, CMDH_OTHER, 0, 0);
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
			CPlayerObject *pIterPlayer = (CPlayerObject*)pIterObj;

			if(pIterPlayer->GetObjectID() == u64CharID)
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  u64CharID, pIterObj->GetObjectID());

			CGameService::GetInstancePtr()->SendCmdToConnection(pIterPlayer->GetConnectID(), pIterPlayer->GetObjectID(), 0, &m_WriteBuffer);

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}


	return TRUE;
}

BOOL CScene::SendRemoveGridsToPlayer( INT32 Grids[9], CPlayerObject *pPlayerObj)
{
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE, CMDH_OTHER, 0, 0);
	
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

			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  pPlayerObj->GetObjectID(), pIterObj->GetObjectID());

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
		return TRUE;
	}

	RemoveFromMap(pPlayerObject);

	m_PlayerObjectMgr.erase(pPlayerObject->GetObjectID());

	return TRUE;
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

	pWorldObject->SetUpdate(UT_Delete);

	return TRUE;

}

BOOL CScene::OnUpdate( UINT32 dwTick )
{
	HandleUpdateList();
	
	return TRUE;
}





BOOL CScene::AddToUpdateList( CWorldObject *pWorldObject )
{
	return m_UpdateObjectMgr.AddUpdateObject(pWorldObject);
}

BOOL CScene::RemoveFromUpList( CWorldObject *pWorldObject )
{
	return m_UpdateObjectMgr.RemoveUpdateObject(pWorldObject);
}

INT32 CScene::OnCmdDBLoadCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBLoadCharInfoAck DBLoadCharInfoAck;
	pBufferHelper->Read(DBLoadCharInfoAck);

	CPlayerObject *pPlayerObject = new CPlayerObject;

	pPlayerObject->LoadFromDBPcket(pBufferHelper);

	pPlayerObject->SetConnectID(DBLoadCharInfoAck.dwProxySvrID);

	m_PlayerObjectMgr.AddPlayer(pPlayerObject);

	if(AddToMap(pPlayerObject))
	{
		StCharEnterGameAck CharEnterGameAck;
		CharEnterGameAck.dwSceneID       = GetSceneID();
		CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_ACK, CMDH_SENCE, 0,  pPlayerObject->GetObjectID());
		WriteHelper.Write(CharEnterGameAck);
		pPlayerObject->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_DEST_MYSELF);
		WriteHelper.EndWrite();
		CGameService::GetInstancePtr()->SendCmdToConnection(DBLoadCharInfoAck.dwProxySvrID, &m_WriteBuffer);
	}
	else
	{
		ASSERT_FAIELD;
		return TRUE;
	}
	
	return TRUE;
}

BOOL CScene::HandleUpdateList()
{
	CWorldObject *pWorldObject = m_UpdateObjectMgr.GetFisrtOjbect();
	while(pWorldObject != NULL)
	{
		HandleUpdateObject(pWorldObject);

		pWorldObject = m_UpdateObjectMgr.GetFisrtOjbect();
	}
	
	return TRUE;
}

BOOL CScene::HandleUpdateObject(CWorldObject *pWorldObject)
{
	if(pWorldObject == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nCurIndex = m_GridManager.GetIndexByPos(pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);
	if(nCurIndex == -1)
	{
		ASSERT_FAIELD;//不合法的地址

		return TRUE;
	}

	if(pWorldObject->m_UpdateType == UT_Unknow)
	{
		ASSERT_FAIELD;
	}
	else if(pWorldObject->m_UpdateType == UT_New)
	{
		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

		SendNewObjectToGrids(pWorldObject, Grids);

		if(pWorldObject->GetObjectType() == OBJECT_PLAYER)
		{
			SendNewGridsToObject(Grids, (CPlayerObject*)pWorldObject);
		}

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateType   = UT_Unknow;
	}
	else if(pWorldObject->m_UpdateType == UT_Delete)
	{
		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), Grids);

		if(pWorldObject->GetObjectType() == OBJECT_PLAYER)
		{
			delete (CPlayerObject*)pWorldObject;
		}

		return TRUE;
	}
	else if(pWorldObject->m_UpdateType == UT_Update)
	{
		INT32 nSrcIndex  = m_GridManager.GetIndexByPos(pWorldObject->m_UpdateObjPos.x, pWorldObject->m_UpdateObjPos.z);

		SendUpdateObjectToMyself(pWorldObject);

		if(nCurIndex == nSrcIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
		{
			INT32 Grids[10];

			m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

			SendUpdateObjectToGrids(pWorldObject, Grids);

		
			return TRUE;
		}

		INT32 AddGrid[10], RemoveGrid[10], StayGrid[10];

		m_GridManager.CalDiffGrids(nSrcIndex, nCurIndex, AddGrid, RemoveGrid, StayGrid);

		//将玩家发的更新发给周围的格子里
		SendUpdateObjectToGrids(pWorldObject, StayGrid);

		SendNewObjectToGrids(pWorldObject, AddGrid);

		SendNewGridsToObject(AddGrid, (CPlayerObject*)pWorldObject);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), RemoveGrid);

		SendRemoveGridsToPlayer(RemoveGrid, (CPlayerObject*)pWorldObject);

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateType   = UT_Unknow;
	}
	
	return TRUE;
}

BOOL CScene::SendUpdateObjectToMyself( CWorldObject *pWorldObj )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_CHAR_UPDATE_MYSELF, CMDH_OTHER, 0, 0);
	pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_DEST_MYSELF);
	WriteHelper.EndWrite();

	CPlayerObject *pPlayerObject = (CPlayerObject *)pWorldObj;

	CGameService::GetInstancePtr()->SendCmdToConnection(pPlayerObject->GetConnectID(), pPlayerObject->GetObjectID(), 0, &m_WriteBuffer);

	return TRUE;
}



