#ifndef _COMMON_CMD_HANDLER_H_
#define _COMMON_CMD_HANDLER_H_
#include "CmdHandler/CommonWorkThread.h"
#include "DataBuffer/DataBuffer.h"

class CCommonCmdHandler : public IThreadCommandHandler
{
public:
	CCommonCmdHandler();

	~CCommonCmdHandler();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick);

	BOOL Init(UINT32 dwReserved);

	BOOL Uninit();

	BOOL AddMessage(UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL OnThreadBegin(){return TRUE;}

	BOOL OnThreadEnd(){return TRUE;}
	

protected:
	CCommonWorkThread				m_WorkThread;

	CDataBuffer<CONST_BUFF_SIZE>	m_WriteBuffer;

	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdDefaultHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	//*********************消息处理定义结束******************************
};

#endif //_COMMON_CMD_HANDLER_H_
