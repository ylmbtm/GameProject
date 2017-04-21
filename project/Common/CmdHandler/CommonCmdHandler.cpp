#include "stdafx.h"
#include "CommonCmdHandler.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"

CCommonCmdHandler::CCommonCmdHandler()
{
	m_WorkThread.SetCommandHandler(this);
}

CCommonCmdHandler::~CCommonCmdHandler()
{

}

BOOL CCommonCmdHandler::Init(UINT32 dwReserved)
{
	m_WorkThread.Start();

	return TRUE;
}

BOOL CCommonCmdHandler::Uninit()
{
	m_WorkThread.Stop();

	return TRUE;
}

BOOL CCommonCmdHandler::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	OnCmdDefaultHandle(wCommandID, u64ConnID, pBufferHelper);
	return TRUE;
}

BOOL CCommonCmdHandler::AddMessage( UINT64 u64ConnID, IDataBuffer *pDataBuffer )
{
	return m_WorkThread.AddMessage(u64ConnID, pDataBuffer);
}



BOOL CCommonCmdHandler::OnCmdDefaultHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{

	return TRUE;
}

BOOL CCommonCmdHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}

