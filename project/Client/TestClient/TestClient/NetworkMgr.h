#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__
#include "IBufferHandler.h"

#define BUFFSIZE 8192

class CNetworkMgr
{
public:
	CNetworkMgr(void);
	virtual ~CNetworkMgr(void);

public:
	static CNetworkMgr* GetInstancePtr();

public:
	BOOL	InitNetSystem();

	BOOL	DisConnect();

	BOOL	IsConnected();

	BOOL	ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL	OnTime();

	BOOL	ReceiveData();

	BOOL	ProcessData();

	BOOL	SendData(CHAR *pData, UINT32 dwLen);

	BOOL	m_bConnected;

	SOCKET	m_hSocket;

	CHAR	m_Buffer[BUFFSIZE];

	IDataBuffer  *m_pReadBuffer;

	IDataBuffer  *m_pWriteBuffer;

	INT32 m_nDataLen;

	DWORD m_nLastError;

	ICommandHandler *m_EngineMsgHandler;
};


#endif