#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__
#include "IBufferHandler.h"

#define BUFFSIZE 8192

enum ConnectState
{
	Not_Connect,
	Start_Connect,
	Raw_Connect,
	Succ_Connect
};

class CNetworkMgr
{
public:
	CNetworkMgr(void);
	virtual ~CNetworkMgr(void);

public:
	BOOL	DisConnect();

	BOOL	ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL	OnTime();

	BOOL	ReceiveData();

	BOOL	ProcessData();

	BOOL    ProcessOnce();

	BOOL	SendData(CHAR *pData, UINT32 dwLen);

	ConnectState m_ConnectState;

	void SetConnectState(ConnectState val);
	SOCKET	m_hSocket;

	CHAR	m_Buffer[BUFFSIZE];

	IDataBuffer  *m_pReadBuffer;

	IDataBuffer  *m_pWriteBuffer;

	INT32 m_nDataLen;

	ICommandHandler *m_EngineMsgHandler;
};


#endif