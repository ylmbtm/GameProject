#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IBufferHandler.h"
#include "Utility/CritSec.h"

#define  NET_CMD_RECV				1
#define  NET_CMD_SEND				2
#define  NET_CMD_CONNECT			3

#define Hash_Map                    std::map

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

    UINT32    GetConnectionID();

    VOID    SetConnectionID(UINT32 dwConnID);

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

    BOOL    ReInit();

public:
	SOCKET						m_hSocket;

	BOOL						m_bConnected;

	NetIoOperatorData			m_IoOverlapRecv;

    UINT32                      m_dwConnID;
    UINT8                       m_byteType;

	IDataHandler				*m_pDataHandler;

	UINT32						m_dwDataLen;
	CHAR						m_pBuffer[CONST_BUFF_SIZE];

	UINT32						m_dwIpAddr;

    CConnection                *m_pNext;
};


class CConnectionMgr
{
private:
	CConnectionMgr();

	~CConnectionMgr();

public:
	static CConnectionMgr* GetInstancePtr();

public:
    BOOL            InitConnectionList(UINT32 nMaxCons);

    CConnection*    CreateConnection();

	VOID		 DeleteConnection(CConnection *pConnection);

    CConnection*    GetConnectionByConnID(UINT32 dwConnID);

    SOCKET          GetConnectionSocket(UINT32 dwConnID);

	///////////////////////////////////////////
	BOOL		 CloseAllConnection();

	BOOL		 DestroyAllConnection();

public:
    CCritSec        m_CritSec;
    CConnection     *m_pFreeConnRoot;
    std::vector<CConnection> m_vtConnList;            //连接列表
};

#endif