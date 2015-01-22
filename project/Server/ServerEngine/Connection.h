#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "Utility/CritSec.h"

#define  NET_CMD_RECV				1
#define  NET_CMD_SEND				2
#define  NET_CMD_CONNECT			3

#define  SVR_CONN_ID				1000

#define  MAGIC_CODE					0x1234


struct NetIoOperatorData
{
#ifdef WIN32
	OVERLAPPED		Overlap;
#endif

	UINT32			dwCmdType;

	IDataBuffer		*pDataBuffer;

	void			Clear();
};

class CConnection
{
public:
	CConnection(void);
	virtual ~CConnection(void);

public:
	BOOL	HandleRecvEvent(UINT32 dwBytes); 

	VOID	SendDisconnectNotify();

	UINT64	GetConnectionID();

	VOID	SetConnectionID(UINT64 dwConnID);

	UINT8	GetConnectionType();

	VOID	SetConnectionType(UINT8 byteType);

	BOOL	Close(BOOL bNotify);

	BOOL	SetSocket(SOCKET hSocket);

	SOCKET  GetSocket();

	BOOL	SetDataHandler(IDataHandler *pHandler);

	BOOL	ExtractBuffer();
	
	BOOL	DoReceive();

	BOOL	IsConnectionOK();

	BOOL	SetConnectionOK(BOOL bOk);

public:
	SOCKET						m_hSocket;

	BOOL						m_bConnected;

	NetIoOperatorData			m_IoOverlapRecv;

	UINT64						m_u64ConnID;
	UINT8						m_byteType;

	IDataHandler				*m_pDataHandler;

	UINT32						m_dwDataLen;
	CHAR						m_pBuffer[CONST_BUFF_SIZE];

	UINT32						m_dwIpAddr;

	UINT32						m_dwMagicCode;
};


class CConnectionMgr
{
private:
	CConnectionMgr();

	~CConnectionMgr();

public:
	static CConnectionMgr* GetInstancePtr();

public:
	CConnection* CreateConnection();

	VOID		 DeleteConnection(CConnection *pConnection);

	CConnection* GetConnectionByConnID(UINT64 dwConnID);

	BOOL		 SetConnectionID(CConnection *pConnection, UINT64 dwConnID);

	///////////////////////////////////////////
	BOOL		 CloseAllConnection();

	BOOL		 DestroyAllConnection();

public:
	CCritSec	m_CritSec;

	UINT32      m_dwNextConnID;

public:
	std::set<CConnection*>			m_WaitConnList;						//存放只是连接成功的连接

	CConnection*					m_StableConnList[SVR_CONN_ID];		//主要存放服务器连接，ID固定，ID范围1-999， 用数据速度快，而且也可以免加锁

	stdext::hash_map<UINT64, CConnection*> m_VarieableConnList;			//主要针对无固定ID的连接
};

#endif