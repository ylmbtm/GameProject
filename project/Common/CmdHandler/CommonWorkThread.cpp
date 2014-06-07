#include "stdafx.h"
#include "CommonWorkThread.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer/BufferHelper.h"
#include "DataBuffer/DataBuffer.h"

CCommonWorkThread::CCommonWorkThread()
{
	m_bRun			= FALSE;

	m_dwLastTick	= CommonFunc::GetTickCount();
}

CCommonWorkThread::~CCommonWorkThread()
{

}

void CCommonWorkThread::Run()
{
	while (m_bRun)
	{
		ProcessMessage();

		UINT32 dwTick = CommonFunc::GetTickCount();
		if((dwTick - m_dwLastTick) >= 50)
		{
			ProcessTimeEvent();
			
			m_dwLastTick = dwTick;
		}

		if(m_MessageQueue.size() <= 0)
		{
			m_pCommandHandler->OnUpdate(dwTick);
		}

		CommonThreadFunc::Sleep(50-dwTick+m_dwLastTick);
		
	}
}

BOOL CCommonWorkThread::Start()
{
	m_bRun = TRUE;

	m_hThread = CommonThreadFunc::CreateThread(_CommonWorkThread, this);

	if(m_hThread != NULL)
	{
		return TRUE;
	}

	return FALSE;
} 

BOOL CCommonWorkThread::Stop()
{
	m_bRun = FALSE;

	CommonThreadFunc::WaitThreadExit(m_hThread);

	return TRUE;
}

BOOL CCommonWorkThread::ProcessTimeEvent()
{
	return TRUE;
}

BOOL CCommonWorkThread::ProcessMessage()
{
	MsgItem msg;

	while(m_MessageQueue.pop(msg))
	{
		if(msg.pDataBuffer == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CBufferHelper BufferReader(FALSE, msg.pDataBuffer);
		if(!BufferReader.BeginRead())
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CommandHeader *pCommandHeader = (CommandHeader *)(msg.pDataBuffer->GetData()+sizeof(TransferHeader));

		m_pCommandHandler->OnCommandHandle(pCommandHeader->wCommandID, msg.u64ConnID, &BufferReader);

		msg.pDataBuffer->Release();
	}

	return TRUE;
}

BOOL CCommonWorkThread::AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer)
{
	IDataBuffer *pRecvBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pDataBuffer->GetDataLenth());
	if(pRecvBuffer == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	pRecvBuffer->CopyFrom(pDataBuffer);

	m_MessageQueue.push(MsgItem(u64ConnID, pRecvBuffer));

	return TRUE;
}

BOOL CCommonWorkThread::SetCommandHandler( ICommandHandler *pCommandHandler )
{
	m_pCommandHandler = pCommandHandler;

	return TRUE;
}


Th_RetName _CommonWorkThread( void *pParam )
{
	CCommonWorkThread *pThread = (CCommonWorkThread *)pParam;

	pThread->Run();

	return Th_RetValue;
}
