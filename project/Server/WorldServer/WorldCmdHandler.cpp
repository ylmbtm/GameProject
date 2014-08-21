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
	CGameService::GetInstancePtr()->SendCmdToDBConnection(&m_WriteBuffer);

	return TRUE;
}

BOOL CWorldCmdHandler::OnCmdDBLoadCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StDBLoadCharInfoAck DBLoadCharInfoAck;
	pBufferHelper->Read(DBLoadCharInfoAck);

	CPlayerObject *pPlayerObject = m_PlayerObjectMgr.CreatePlayerByID(DBLoadCharInfoAck.u64CharID);
	CHECK_RETURN_FALSE_A(pPlayerObject);

	pPlayerObject->LoadFromDBPcket(pBufferHelper);

	

	//继续往游戏服转移
	StSvrEnterSceneReq SvrEnterSceneReq;
	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);
	WriteHelper.BeginWrite(CMD_SVR_ENTER_SCENE_REQ, CMDH_OTHER, 0, 0);
	WriteHelper.Write(SvrEnterSceneReq);
	WriteHelper.EndWrite();
	CGameService::GetInstancePtr()->SendCmdToConnection(DBLoadCharInfoAck.dwProxySvrID, &m_WriteBuffer);

	return TRUE;
}



