#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_
#include "IBufferHandler.h"
#include "Utility/CommonThreadFunc.h"
#include "Utility/CommonMsgQueue.h"

Th_RetName _NetEventThread( void *pParam );

Th_RetName _NetListenThread( void *pParam );

Th_RetName _NetEventDispatchThread(void *pParam ); //only for linux

Th_RetName _NetSendBufferThread(void *pParam);

struct SendDataNode
{
	BOOL		bIsConnID;	//true is id ,flase if socket
	UINT64		u64ConnID;
	VOID		*pPtr;

	SendDataNode()
	{
		bIsConnID = false;
		u64ConnID  = 0;
		pPtr      = NULL;
	}
};

////以下为linux专有//////////////////////////////
#define EVENT_READ  1
#define EVENT_WRITE 2

struct EventNode
{
	UINT32		dwEvent;
	void		*pPtr;
};

////////////////////////////////////////////////

class CNetManager
{
	CNetManager(void);

	virtual ~CNetManager(void);
public:
	static CNetManager* GetInstancePtr()
	{
		static CNetManager NetManager;

		return &NetManager;
	}
public:
	BOOL	Start(IDataHandler *pBufferHandler);

	BOOL	Close();

	BOOL	SendBufferByConnID(UINT64 u64ConnID, IDataBuffer *pDataBuffer);

	BOOL	SendBufferBySocket(SOCKET hSocket, IDataBuffer *pDataBuffer);

public:
	BOOL	InitNetwork();

	BOOL	UninitNetwork();

	BOOL	StartListen();

	BOOL	StopListen();

	//以下是完成端口部分
public:
	BOOL	CreateCompletePort();

	CConnection* AssociateCompletePort(SOCKET hSocket);

	BOOL	DestroyCompletePort();

	BOOL	CreateDispatchThread();

	BOOL    CloseDispatchThread();
	
	BOOL	CreateEventThread(int nNum);

	BOOL    CloseEventThread();
	
	BOOL	CreateSendDataThread();

	BOOL	CloseSendDataThread();

	BOOL	WorkThread_DispathEvent();

	BOOL	WorkThread_ProcessEvent();

	BOOL	WorkThread_Listen();

	BOOL    WorkThread_SendData();

	BOOL	ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	BOOL	ConnectToOtherSvrEx(std::string strIpAddr, UINT16 sPort);

	BOOL	SendIdentifyInfo(SOCKET hSocket);

	SOCKET				m_hListenSocket;

	HANDLE				m_hCompletePort;

	BOOL				m_bCloseSend;		//是否关闭发送数据的线程

	BOOL				m_bCloseEvent;		//是否关闭事件处理线程

	IDataHandler		*m_pBufferHandler;

public:

	CommonQueue::CMessageQueue<EventNode>	m_DispatchEventList;
	
	CommonQueue::CMessageQueue<SendDataNode, 1024> m_SendDataList;		//用于发送数据的线程



	THANDLE				m_hListenThread;
	THANDLE				m_hSendThread;
	THANDLE				m_hDispathThread;
	std::vector<THANDLE> m_vtEventThread;

#ifndef WIN32

	static void SignalHandler(int nValue)
	{

	}

	BOOL  ClearSignal()
	{
		m_NewAct.sa_handler = CNetManager::SignalHandler;

		sigemptyset(&m_NewAct.sa_mask); //清空此信号集

		m_NewAct.sa_flags = 0;

		sigaction(SIGPIPE, &m_NewAct, &m_OldAct);

		return TRUE;
	}

	BOOL RestoreSignal()
	{
		sigaction(SIGPIPE, &m_OldAct, NULL); //恢复成原始状态

		return TRUE;
	}

	struct sigaction m_NewAct, m_OldAct;

#endif

};

#endif

