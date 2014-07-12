#pragma once
#ifndef __CLIENT_ENGINE_H__
#define __CLIENT_ENGINE_H__
#include "DataBuffer\DataBuffer.h"
#include "GameDefine.h"

enum ConnectState
{
	Not_Connect,
	Start_Connect,
	Raw_Connect,
	Succ_Connect
};


struct IMessageHandler
{
	virtual BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper) = 0;
};

class ClientEngine : public ICommandHandler
{
private:
	ClientEngine();

	~ClientEngine();

public:
	static ClientEngine* GetInstancePtr();

	BOOL	InitEngine();

	BOOL	CloseEngine();

	BOOL	SetClientID(UINT64 u64ClientID);  //一般是角色ID

	BOOL	ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL	DisConnect();

	BOOL	SendData(char *pData, INT32 dwLen);
	
	BOOL	Login(const char *pszAccountName, const char *pszPassword);

	BOOL	RegisterMsgHandler(IMessageHandler *pMsgHandler);

	BOOL	UnregisterMsgHandler(IMessageHandler *pMsgHandler);

	BOOL	Render();
	
	IDataBuffer* GetWriteBuffer();

	UINT32  GetServerTime();

protected:
	VOID	SetConnectState(ConnectState val);

	BOOL	ReceiveData();

	BOOL	ProcessData();

	//以下是内部的消息处理
	BOOL	OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL	OnUpdate(UINT32 dwTick){return TRUE;};

	BOOL	OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL	OnCmdPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper );

	BOOL	OnCmdHearBeatAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper );
protected:
	UINT64				m_u64ClientID;
	UINT32				m_dwIdentifyCode;

	ConnectState		m_ConnectState;

	SOCKET	m_hSocket;

	INT32   m_nDataLen;
	CHAR	m_DataBuffer[CONST_BUFF_SIZE];

	CDataBuffer<CONST_BUFF_SIZE>  m_ReadBuffer;

	CDataBuffer<CONST_BUFF_SIZE>  m_WriteBuffer;
	
	std::vector<IMessageHandler*> m_vtMsgHandler;


	UINT32				m_dwServerTime;
	UINT32				m_dwServerTick;

};

#endif
