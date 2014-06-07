#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__


#include "IBufferHandler.h"

#define BUFFSIZE 8192

class CNetworkMgr
{
private:
	CNetworkMgr(void);
	virtual ~CNetworkMgr(void);

public:
	static CNetworkMgr* GetInstancePtr();
public:

	BOOL   InitNetSystem();

	BOOL   DisConnect();

	BOOL   IsConnected();

	BOOL   ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL   OnTime();

	BOOL   RecvData();

	BOOL   ProcessData();

	BOOL   SendData(char *pData, UINT32 dwLen);

	BOOL   m_bConnected;

	SOCKET m_hSocket;

	CHAR  m_Buffer[BUFFSIZE];

	IDataBuffer  *m_pReadBuffer;

	IDataBuffer  *m_pWriteBuffer;

	int m_nDataLen;

	DWORD m_nLastError;
};


#endif