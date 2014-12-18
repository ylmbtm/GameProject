#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "Error.h"
#include "WorldCmdHandler.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "PacketDef/ClientPacket.h"
#include "ConnectionType.h"
#include "GameService.h"
#include "PacketDef/PacketHeader.h"
#include "PacketDef/DBPacket.h"
#include "Utility/CommonSocket.h"
#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketDef/SvrReportPacket.h"
#include "PlayerObject.h"




CWorldCmdHandler::CWorldCmdHandler()
{

}

CWorldCmdHandler::~CWorldCmdHandler()
{

}

BOOL CWorldCmdHandler::Init(UINT32 dwReserved)
{
	if(!CCommonCmdHandler::Init(dwReserved))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CWorldCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CWorldCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_CHAR_ENTER_GAME_REQ,	OnCmdEnterGameReq);
		PROCESS_COMMAND_ITEM(CMD_DB_LOAD_CHAR_ACK,		OnCmdDBLoadCharAck);
		PROCESS_COMMAND_ITEM(CMD_SVR_CREATE_SCENE_ACK,	OnCmdCreateSceneAck);
		PROCESS_COMMAND_ITEM(CMD_CHAR_LEAVE_GAME_REQ,	OnCmdLeaveGameReq);
	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CWorldCmdHandler::OnUpdate( UINT32 dwTick )
{


	return TRUE;
}

BOOL CWorldCmdHandler::OnCmdEnterGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameReq CharEnterGameReq;
	pBufferHelper->Read(CharEnterGameReq);

	StDBLoadCharInfoReq DBLoadCharInfoReq;
	DBLoadCharInfoReq.u64CharID		= CharEnterGameReq.u64CharID;
	DBLoadCharInfoReq.dwProxySvrID	= (UINT32)u64ConnID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_DB_LOAD_CHAR_REQ, CMDH_OTHER, 0, 0);
	WriteHelper.Write(DBLoadCharInfoReq);
	WriteHelper.EndWrite();
	ASSERT(m_WriteBuffer.GetDataLenth() >= (sizeof(DBLoadCharInfoReq)+22));
	if (!CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer))
	{
		ASSERT_FAIELD;
	}

	return TRUE;
}

BOOL CWorldCmdHandler::OnCmdLeaveGameReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLeaveGameReq CharLeaveGameReq;

	pBufferHelper->Read(CharLeaveGameReq);

	if(CharLeaveGameReq.dwLeaveReason == LGR_Disconnect)
	{

	}
	else if(CharLeaveGameReq.dwLeaveReason == LGR_Quit)
	{

	}

	CPlayerObject *pPlayerObject = m_PlayerObjectMgr.GetPlayer(pBufferHelper->GetCommandHeader()->u64CharID);
	if(pPlayerObject == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	m_PlayerObjectMgr.ReleasePlayer(pPlayerObject->GetObjectID());

	return TRUE;
}


BOOL CWorldCmdHandler::OnCmdDBLoadCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBLoadCharInfoAck DBLoadCharInfoAck;
	pBufferHelper->Read(DBLoadCharInfoAck);

	CPlayerObject *pPlayerObject = m_PlayerObjectMgr.CreatePlayerByID(DBLoadCharInfoAck.u64CharID);
	CHECK_AND_RETURN_ASSERT(pPlayerObject, TRUE);

	if(!pPlayerObject->LoadFromDBPacket(pBufferHelper))
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	UINT32 dwGameSvrID = m_GameSvrMgr.GetServerIDBySceneID(pPlayerObject->m_dwSceneID);
	if(dwGameSvrID == 0)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	//继续往游戏服转移
	StSvrEnterSceneReq SvrEnterSceneReq;
	SvrEnterSceneReq.dwGameSvrID  = dwGameSvrID;
	SvrEnterSceneReq.dwProxySvrID = DBLoadCharInfoAck.dwProxySvrID;
	SvrEnterSceneReq.dwSceneID    = pPlayerObject->m_dwSceneID;
	SvrEnterSceneReq.u64CharID    = DBLoadCharInfoAck.u64CharID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_SVR_ENTER_SCENE_REQ, CMDH_OTHER, SvrEnterSceneReq.dwSceneID, DBLoadCharInfoAck.u64CharID);
	WriteHelper.Write(SvrEnterSceneReq);

	pPlayerObject->SaveDataToPacket(&WriteHelper);

	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(SvrEnterSceneReq.dwGameSvrID, &m_WriteBuffer);

	return TRUE;
}

BOOL CWorldCmdHandler::OnCmdCreateSceneAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StSvrCreateSceneAck CreateSceneAck;
	pBufferHelper->Read(CreateSceneAck);

	if(CreateSceneAck.dwAckCode == E_SUCCESSED)
	{
		//创建场景成功 
	}
	else
	{
		//创建场景失败
	}


	return TRUE;
}

