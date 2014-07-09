#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "Error.h"
#include "StatCmdHandler.h"
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





CStatCmdHandler::CStatCmdHandler()
{

}

CStatCmdHandler::~CStatCmdHandler()
{

}

BOOL CStatCmdHandler::Init(UINT32 dwReserved)
{
	if(!CCommonCmdHandler::Init(dwReserved))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CStatCmdHandler::Uninit()
{
	CCommonCmdHandler::Uninit();

	return TRUE;
}

BOOL CStatCmdHandler::OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM(CMD_SVR_RUNNING_STATE_REPORT,	OnCmdSvrRunningStateReq);
	default:
		{

		}
		break;
	}

	return TRUE;
}

BOOL CStatCmdHandler::OnUpdate( UINT32 dwTick )
{


	return TRUE;
}

BOOL CStatCmdHandler::OnThreadBegin()
{

	return TRUE;
}

BOOL CStatCmdHandler::OnThreadEnd()
{
	return TRUE;
}



BOOL CStatCmdHandler::OnCmdSvrRunningStateReq( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwReportID = 0;
	pBufferHelper->Read(dwReportID);
	pBufferHelper->Seek(0 - sizeof(dwReportID));

	switch(dwReportID)
	{
	case SVR_REPROT_NORMAL:
		{
			OnSvrReport_NormalLog(pBufferHelper);
		}
		break;
	default:
		{

		}
		break;
	}


	return TRUE;
}

BOOL CStatCmdHandler::OnSvrReport_NormalLog( CBufferHelper *pBufferHelper )
{
	StNormalReport NormalReport;
	pBufferHelper->Read(NormalReport);

	printf("SVRID:%d, SVRTYPE:%d, REPORTID:%d, CONTENT:%s", NormalReport.dwSvrID, NormalReport.dwSvrType, NormalReport.dwReportID, NormalReport.szLogContent);

	return TRUE;
}



