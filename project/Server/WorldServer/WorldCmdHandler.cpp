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



