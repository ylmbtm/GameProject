#include "stdafx.h"
#include "NetManager.h"
#include "GlobalConfig.h"
#include "Connection.h"
#include "Utility/CommonSocket.h"
#include "Utility/CommonFunc.h"
#include "PacketDef/ClientPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"
#include "DataBuffer/DataBuffer.h"


CNetManager::CNetManager(void)
{
	m_hListenThread		= (THANDLE)NULL;
	m_hSendThread		= (THANDLE)NULL;

	m_hListenSocket		= NULL;

	m_hCompletePort		= NULL;

	m_bCloseSend		= TRUE;

	m_bCloseEvent		= TRUE;

	m_pBufferHandler	= NULL;
}

CNetManager::~CNetManager(void)
{
}

BOOL CNetManager::CreateEventThread( int nNum )
{
	if(nNum == 0)
	{
		nNum = CommonFunc::GetProcessorNum();
	}

	if(nNum <= 0)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_bCloseEvent = FALSE;

	for(int i = 0; i < nNum; ++i)
	{
		THANDLE hThread = CommonThreadFunc::CreateThread(_NetEventThread, (void*)this); 
		m_vtEventThread.push_back(hThread);
	}

	return TRUE;
}

BOOL CNetManager::WorkThread_Listen()
{
	while(TRUE)
	{
		sockaddr_in Con_Addr;
		socklen_t nLen = sizeof(Con_Addr);
		memset(&Con_Addr, 0, sizeof(Con_Addr));
		SOCKET hClientSocket = accept(m_hListenSocket, (sockaddr*)&Con_Addr, &nLen);
		if(hClientSocket == INVALID_SOCKET)
		{
			CLog::GetInstancePtr()->AddLog("accept 错误 原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());

			break;
		}

		CommonSocket::SetSocketUnblock(hClientSocket);

		CConnection *pConnection = AssociateCompletePort(hClientSocket);
		if(pConnection != NULL)
		{
			CLog::GetInstancePtr()->AddLog("成功收到新连接，发送身份信息, 并提交数据请求!");

			pConnection->SetConnectionOK(TRUE);

			SendIdentifyInfo(hClientSocket);

#ifdef WIN32
			if(!pConnection->DoReceive())
			{
				pConnection->Close(FALSE);

				CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
			}
#endif
		}
		else
		{
			CLog::GetInstancePtr()->AddLog("accept 错误 原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		}
	}

	return TRUE;
}


BOOL CNetManager::StartListen()
{
	sockaddr_in SvrAddr;
	SvrAddr.sin_family		= AF_INET;
	SvrAddr.sin_port		= htons(CGlobalConfig::GetInstancePtr()->m_sPort);
	SvrAddr.sin_addr.s_addr	= htonl(INADDR_ANY);		//支持多IP地址监听
	//inet_pton(AF_INET, CGlobalConfig::GetInstancePtr()->m_strIpAddr.c_str(), &SvrAddr.sin_addr);

	m_hListenSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(m_hListenSocket == INVALID_SOCKET)
	{
		CLog::GetInstancePtr()->AddLog("创建监听套接字失败原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}
	
	CommonSocket::SetSocketReuseable(m_hListenSocket);

	if(!CommonSocket::BindSocket(m_hListenSocket, (sockaddr*)&SvrAddr, sizeof(SvrAddr)))
	{
		CLog::GetInstancePtr()->AddLog("邦定套接字失败原因:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if(!CommonSocket::ListenSocket(m_hListenSocket, 20))
	{
		CLog::GetInstancePtr()->AddLog("监听线程套接字失败:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	if((m_hListenThread = CommonThreadFunc::CreateThread(_NetListenThread,  (void*)NULL)) == NULL)
	{
		CLog::GetInstancePtr()->AddLog("创建监听线程失败:%s!", CommonSocket::GetLastErrorStr(CommonSocket::GetSocketLastError()).c_str());
		return FALSE;
	}

	return TRUE;
}


BOOL	CNetManager::CreateSendDataThread()
{
	m_bCloseSend = FALSE;

	if((m_hSendThread = CommonThreadFunc::CreateThread(_NetSendBufferThread,  (void*)NULL))==NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

#ifdef WIN32

BOOL CNetManager::WorkThread_ProcessEvent()
{
	if(m_hCompletePort == INVALID_HANDLE_VALUE)
	{
		CLog::GetInstancePtr()->AddLog("创建事件网络事件处理线程失败， 完成端口还未创建!");
		ASSERT_FAIELD;
		return FALSE;
	}

	DWORD dwNumOfByte = 0;
	DWORD dwCompleteKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	DWORD dwWaitTime = 1000;
	BOOL  bRetValue = FALSE;

	while(!m_bCloseEvent)
	{
		bRetValue = GetQueuedCompletionStatus(m_hCompletePort, &dwNumOfByte, &dwCompleteKey, &lpOverlapped, dwWaitTime);
		if(!bRetValue)
		{
			if(lpOverlapped == NULL)
			{ 
				if(ERROR_ABANDONED_WAIT_0 == CommonSocket::GetSocketLastError())
				{
					CLog::GetInstancePtr()->AddLog("完成端口被外部关闭!");
					return FALSE;
				}

				if(CommonSocket::GetSocketLastError() == WAIT_TIMEOUT)
				{
					continue;
				}
			}
		}

		NetIoOperatorData *pIoPeratorData = (NetIoOperatorData*)lpOverlapped;
		switch( pIoPeratorData->dwCmdType )
		{
		case NET_CMD_RECV:
			{
				CConnection *pConnection = (CConnection *)dwCompleteKey;
				if(pConnection != NULL)
				{
					if(dwNumOfByte == 0)
					{
						//说明对方己经关闭
						CLog::GetInstancePtr()->AddLog("完成端口收到数据为0, 对方己经关闭连接:连接指针%x!", pConnection);
						pConnection->Close(TRUE);
						CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
					}
					else
					{
						if(!pConnection->HandleRecvEvent(dwNumOfByte))
						{
							//收数据失败，基本就是连接己断开
							pConnection->Close(TRUE);

							CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
						}
					}
				}
			}
			break;
		case NET_CMD_SEND:
			{
				pIoPeratorData->pDataBuffer->Release();
			}
			break;
		case NET_CMD_CONNECT:
			{
				CConnection *pConnection = (CConnection *)dwCompleteKey;
				if(pConnection != NULL)
				{
					if(bRetValue)
					{
						CLog::GetInstancePtr()->AddLog("连接其它服务器成功!");

						pConnection->SetConnectionOK(TRUE);

						if(SendIdentifyInfo(pConnection->GetSocket()))
						{
							if(!pConnection->DoReceive())
							{
								pConnection->Close(FALSE);

								CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
							}
						}
					}
					else
					{
						CLog::GetInstancePtr()->AddLog("连接其它服务器失败!");

						pConnection->Close(FALSE);

						CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
					}
				}
			}
			break;
		default:
			{

			}
			break;
		}
	}

	return TRUE;
}

BOOL    CNetManager::WorkThread_SendData()
{
	while(!m_bCloseSend)
	{
		SendDataNode _SendNode;
		
		if(!m_SendDataList.Pop(_SendNode))
		{
			continue;
		}
	
		IDataBuffer *pDataBuffer = (IDataBuffer *)_SendNode.pPtr;
		if(pDataBuffer == NULL)
		{
			if(_SendNode.u64ConnID != 0)
			{
				ASSERT_FAIELD;
				CLog::GetInstancePtr()->AddLog("发送线程:发送失败 ，一个空包!");
			}
		
			CLog::GetInstancePtr()->AddLog("发送线程:遇到一个退出线程包!");
			continue;
		}

		SOCKET hSocket = INVALID_SOCKET;
		CConnection *pConnection = NULL;
		if(_SendNode.bIsConnID)
		{
			pConnection = CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(_SendNode.u64ConnID);
			if(pConnection == NULL)
			{
				CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 无效的连接ID：%lld!", _SendNode.u64ConnID);

				pDataBuffer->Release();

				//ASSERT_FAIELD;  这里可能是正常的情况，因为连接己经断开了，所以找不到链接的ID了。

				continue;
			}

			hSocket = pConnection->GetSocket();
		}
		else
		{
			hSocket = (SOCKET)_SendNode.u64ConnID;
		}

		if(hSocket == INVALID_SOCKET)
		{
			CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 无效的套接字!");

			pDataBuffer->Release();

			ASSERT_FAIELD;

			continue;
		}

		WSABUF  DataBuf;
		DataBuf.len = pDataBuffer->GetDataLenth();
		DataBuf.buf = pDataBuffer->GetData();

		NetIoOperatorData *pOperatorData = (NetIoOperatorData *)pDataBuffer->GetBufferPos(pDataBuffer->GetDataLenth()+1);
		if(pOperatorData == NULL)
		{
			pDataBuffer->Release();
			ASSERT_FAIELD;
			continue;
		}

		pOperatorData->Clear();
		pOperatorData->dwCmdType   = NET_CMD_SEND;
		pOperatorData->pDataBuffer = pDataBuffer;

		DWORD dwSendBytes;
		int nRet = WSASend(hSocket, &DataBuf, 1, &dwSendBytes, 0, (LPOVERLAPPED)pOperatorData, NULL);
		if(nRet == 0) //发送成功
		{
			//if(dwSendBytes < DataBuf.len)
			if(dwSendBytes == 0)
			{
				if(pConnection != NULL)
				{
					CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 所以主动关闭连接%lld!, 连接指针%x", pConnection->GetConnectionID(), pConnection);

					pConnection->Close(TRUE);

					CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
				}
				else
				{
					CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 未能发送出数据，连接为空!");
				}
				
				pDataBuffer->Release();
			}
		}
		else if( nRet == -1 ) //发送出错
		{
			UINT32 errCode = CommonSocket::GetSocketLastError();
			if(errCode != ERROR_IO_PENDING)
			{
				if(pConnection != NULL)
				{
					pConnection->Close(TRUE);

					CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
				}

				pDataBuffer->Release();

				CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 连接关闭原因:%s!", CommonSocket::GetLastErrorStr(errCode).c_str());
			}
		}
	}

	return TRUE;
}


BOOL CNetManager::CreateCompletePort()
{
	m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, -1);
	if(m_hCompletePort == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket )
{
	CConnection *pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();
	if(pConnection == NULL)
	{
		ASSERT_FAIELD;
		return NULL;
	}

	pConnection->SetSocket(hSocket);

	pConnection->SetDataHandler(m_pBufferHandler);

	if(NULL == CreateIoCompletionPort((HANDLE)hSocket, m_hCompletePort, (ULONG_PTR)pConnection, 0))
	{
		pConnection->Close(FALSE);

		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
		ASSERT_FAIELD;

		return NULL;
	}

	return pConnection;
}

BOOL CNetManager::DestroyCompletePort()
{
	CloseHandle(m_hCompletePort);

	return TRUE;
}

BOOL CNetManager::CreateDispatchThread()
{
	return TRUE;
}

BOOL CNetManager::WorkThread_DispathEvent()
{
	return TRUE;
}

#else

BOOL    CNetManager::WorkThread_SendData()
{
	while(!m_bCloseSend)
	{
		SendDataNode _SendNode;

		if(!m_SendDataList.Pop(_SendNode))
		{
			continue;
		}

		IDataBuffer *pDataBuffer = (IDataBuffer *)_SendNode.pPtr;
		if(pDataBuffer == NULL)
		{
			if(_SendNode.u64ConnID != 1)
			{
				CLog::GetInstancePtr()->AddLog("发送线程:空消息 ，准备退出发送线程!");
			}

			continue;
		}

		SOCKET hSocket = INVALID_SOCKET;
		CConnection *pConnection = NULL;
		if(_SendNode.bIsConnID)
		{
			pConnection = CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(_SendNode.u64ConnID);
			if(pConnection == NULL)
			{
				CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 无效的连接ID!");

				pDataBuffer->Release();

				ASSERT_FAIELD;
				continue;
			}

			hSocket = pConnection->GetSocket();
		}
		else
		{
			hSocket = _SendNode.u64ConnID;
		}

		if(hSocket == INVALID_SOCKET)
		{
			CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 无效的套接字!");

			pDataBuffer->Release();

			ASSERT_FAIELD;
			continue;
		}

		INT32 nRet = send(hSocket, pDataBuffer->GetData(),pDataBuffer->GetDataLenth(), 0);
		if(nRet < 0)
		{
			int nErr = CommonSocket::GetSocketLastError();

			CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 原因:%s!", CommonSocket::GetLastErrorStr(nErr).c_str());
		}
		else if(nRet < pDataBuffer->GetDataLenth())
		{
			if(pConnection != NULL)
			{
				CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 所以主动关闭连接%lld!, 连接指针%x", pConnection->GetConnectionID(), pConnection);

				pConnection->Close(TRUE);

				CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
			}

			CLog::GetInstancePtr()->AddLog("发送线程:发送失败, 缓冲区满了!");
		}

		pDataBuffer->Release();
	}

	return TRUE;
}

BOOL CNetManager::CreateCompletePort()
{
	m_hCompletePort = epoll_create(MAXEPOLLSIZE);
	if(m_hCompletePort == -1)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}

CConnection* CNetManager::AssociateCompletePort( SOCKET hSocket )
{
	CConnection *pConnection = CConnectionMgr::GetInstancePtr()->CreateConnection();

	pConnection->SetSocket(hSocket);

	pConnection->SetDataHandler(m_pBufferHandler);

	struct epoll_event EpollEvent;
	EpollEvent.data.ptr= pConnection;
	EpollEvent.events  = EPOLLIN|EPOLLOUT|EPOLLET;

	if(-1 == epoll_ctl(m_hCompletePort, EPOLL_CTL_ADD, hSocket,&EpollEvent))
	{
		pConnection->Close(FALSE);

		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);

		return NULL;
	}

	return pConnection;
}

BOOL CNetManager::DestroyCompletePort()
{
	close(m_hCompletePort);

	return TRUE;
}

BOOL CNetManager::CreateDispatchThread()
{
	m_bCloseDispath = FALSE;

	m_hDispathThread = CommonThreadFunc::CreateThread(_NetEventDispatchThread,  (void*)NULL);
	if(m_hDispathThread != (THANDLE)NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	return FALSE;
}

BOOL CNetManager::WorkThread_DispathEvent()
{
	struct epoll_event EpollEvent[20];
	int nFd = 0;
	EventNode _EventNode;
	while(!m_bCloseDispath)
	{
		nFd = epoll_wait(m_hCompletePort, EpollEvent, 20, 1000);

		for(int i = 0; i < nFd; ++i)
		{
			CConnection *pConnection = (CConnection*)EpollEvent[i].data.ptr;
			if(pConnection == NULL)
			{
				continue ;
			}

			if((EpollEvent[i].events & EPOLLERR)||(EpollEvent[i].events & EPOLLHUP))
			{
				if(!pConnection->IsConnectionOK())
				{
					CLog::GetInstancePtr()->AddLog("---未连接socket收到这个消息----EPOLLERR------------!");
					continue;
				}
				
				CLog::GetInstancePtr()->AddLog("---己连接socket收到这个消息----EPOLLERR------------!");

				continue;
			}
			
			int nError;
			socklen_t len;

			if(getsockopt(pConnection->GetSocket(), SOL_SOCKET, SO_ERROR, (char*)&nError, &len) < 0)
			{
				CLog::GetInstancePtr()->AddLog("-------getsockopt Error:%d--------成功----!", nError);
				continue;
			}

			if(EpollEvent[i].events & EPOLLIN)
			{
				if(nError ==0)
				{
					_EventNode.dwEvent = EVENT_READ;

					_EventNode.pPtr = EpollEvent[i].data.ptr;

					m_DispatchEventList.Push(_EventNode);

					CLog::GetInstancePtr()->AddLog("-------EPOLLIN--------成功----!");
				}
				else
				{
					CLog::GetInstancePtr()->AddLog("-------EPOLLIN---------失败---!");
				}
			}
			
			if(EpollEvent[i].events & EPOLLOUT)
			{
				if(nError == 0)
				{
					if(!pConnection->IsConnectionOK())
					{
						pConnection->SetConnectionOK(TRUE);

						SendIdentifyInfo(pConnection->GetSocket());

						CLog::GetInstancePtr()->AddLog("-------EPOLLOUT-----成功---发送身份信息----!");
					}

					CLog::GetInstancePtr()->AddLog("-------EPOLLOUT-----成功-------!");
				}
				else
				{
					CLog::GetInstancePtr()->AddLog("-------EPOLLOUT----失败-------!");
				}
			}
		}
	}

	return TRUE;
}

BOOL CNetManager::WorkThread_ProcessEvent()
{
	EventNode _EventNode;
	while(TRUE)
	{
		if(!m_DispatchEventList.Pop(_EventNode))
		{
			continue;
		}

		CConnection *pConnection = (CConnection *)_EventNode.pPtr;
		if(pConnection == NULL)
		{
			if(_EventNode.dwEvent != 1)
			{
				CLog::GetInstancePtr()->AddLog("错误:取出一个空事件!");
			}

			continue;
		}

		if(_EventNode.dwEvent == EVENT_READ)
		{
			if(!pConnection->HandleRecvEvent(0))
			{
				//基本表明连接己断开，可以关闭连接了。
				pConnection->Close(TRUE);

				CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
			}
			else
			{
				struct epoll_event EpollEvent;
				EpollEvent.data.ptr= pConnection;
				EpollEvent.events  = EPOLLIN|EPOLLET;

				epoll_ctl(m_hCompletePort, EPOLL_CTL_MOD, pConnection->GetSocket(),  &EpollEvent);
			}
		}
		else if(_EventNode.dwEvent == EVENT_WRITE)
		{

		}
	}

	return TRUE;
}


#endif


BOOL CNetManager::Start( IDataHandler *pBufferHandler )
{
	if(pBufferHandler == NULL)
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	m_pBufferHandler = pBufferHandler;

	if(!InitNetwork())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("初始化网络失败！！");
		return FALSE;
	}

	if(!CreateCompletePort())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("创建完成端口或Epoll失败！！");
		return FALSE;
	}

	if(!CreateEventThread(0))
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("创建网络事件处理线程失败！！");
		return FALSE;
	}

	if(!CreateDispatchThread())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("创建事件分发线程失败！！");
		return FALSE;
	}

	if(!CreateSendDataThread())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("创建数据发送线程失败！！");
		return FALSE;
	}

	if(!StartListen())
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("开启监听失败！！");
		return FALSE;
	}

	return TRUE;
}

BOOL CNetManager::InitNetwork()
{
	return CommonSocket::InitNetwork();
}

BOOL CNetManager::UninitNetwork()
{
	return CommonSocket::UninitNetwork();
}

BOOL CNetManager::Close()
{
	StopListen();

	CloseDispatchThread();

	CloseEventThread();
	
	CloseSendDataThread();

	CConnectionMgr::GetInstancePtr()->CloseAllConnection();

	DestroyCompletePort();

	UninitNetwork();

	CConnectionMgr::GetInstancePtr()->DestroyAllConnection();

	return TRUE;
}

BOOL CNetManager::StopListen()
{
	CommonSocket::CloseSocket(m_hListenSocket);

	CommonThreadFunc::WaitThreadExit(m_hListenThread);

	return TRUE;
}

BOOL CNetManager::ConnectToOtherSvr( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		ASSERT_FAIELD;
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->AddLog("创建套接字失败!!");
		return FALSE;
	}

	CommonSocket::SetSocketBlock(hSocket);

	CommonSocket::SetSocketNoDelay(hSocket);

	if(!CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort))
	{
		CommonSocket::CloseSocket(hSocket);
		return FALSE;
	}

	CConnection *pConnection = AssociateCompletePort(hSocket);
	if(pConnection == NULL)
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("邦定套接字到完成端口失败!!");

		return FALSE;
	}

	SendIdentifyInfo(hSocket);

	if(!pConnection->DoReceive())
	{
		pConnection->Close(FALSE);

		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);
	}

	return TRUE;
}

BOOL CNetManager::ConnectToOtherSvrEx( std::string strIpAddr, UINT16 sPort )
{
	SOCKET hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(hSocket == INVALID_SOCKET)
	{
		ASSERT_FAIELD;
		CommonSocket::CloseSocket(hSocket);
		CLog::GetInstancePtr()->AddLog("创建套接字失败!!");
		return FALSE;
	}

	CommonSocket::SetSocketUnblock(hSocket);

	CommonSocket::SetSocketNoDelay(hSocket);

	CConnection *pConnection = AssociateCompletePort(hSocket);
	if(pConnection == NULL)
	{
		CLog::GetInstancePtr()->AddLog("邦定套接字到完成端口失败!!");

		return FALSE;
	}

#ifdef WIN32

	pConnection->m_IoOverlapRecv.Clear();

	pConnection->m_IoOverlapRecv.dwCmdType = NET_CMD_CONNECT;

	BOOL bRet = CommonSocket::ConnectSocketEx(hSocket, strIpAddr.c_str(), sPort, (LPOVERLAPPED)&pConnection->m_IoOverlapRecv);

#else

	BOOL bRet = CommonSocket::ConnectSocket(hSocket, strIpAddr.c_str(), sPort);

#endif
	if(!bRet)
	{
		pConnection->Close(FALSE);

		CConnectionMgr::GetInstancePtr()->DeleteConnection(pConnection);

		CLog::GetInstancePtr()->AddLog("连接服务器%s : %d失败!!", strIpAddr.c_str(), sPort);

		return FALSE;
	}

	pConnection->m_dwIpAddr = CommonSocket::IpAddrStrToInt((CHAR*)strIpAddr.c_str());

	return TRUE;
}


BOOL CNetManager::SendIdentifyInfo(SOCKET hSocket)
{
	StConnectNotify ConnectNotify;

	IDataBuffer *pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(1024);
	if(pDataBuffer == NULL)
	{
		ASSERT_FAIELD;
		CLog::GetInstancePtr()->AddLog("申请内存块失败!!");
		return FALSE;
	}

	CBufferHelper BufferWriter(TRUE, pDataBuffer);

	BufferWriter.BeginWrite(CMD_CONNECT_NOTIFY, CMDH_SVR_CON, 0, 0);

	ConnectNotify.u64ConnID = CGlobalConfig::GetInstancePtr()->m_dwServerID;

	ConnectNotify.btConType = CGlobalConfig::GetInstancePtr()->m_dwServerType;

	BufferWriter.Write(ConnectNotify);

	BufferWriter.EndWrite();

	SendBufferBySocket(hSocket, pDataBuffer);

	return TRUE;
}


BOOL	CNetManager::SendBufferByConnID(UINT64 u64ConID, IDataBuffer *pDataBuffer)
{
	if((pDataBuffer == NULL)||(u64ConID == 0))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	SendDataNode _SendNode;

	_SendNode.bIsConnID		= true;

	_SendNode.u64ConnID		= u64ConID;

	_SendNode.pPtr			= pDataBuffer;

	return m_SendDataList.Push(_SendNode);
}

BOOL	CNetManager::SendBufferBySocket(SOCKET hSocket, IDataBuffer *pDataBuffer)
{
	if((pDataBuffer == NULL)||(hSocket == INVALID_SOCKET)||(hSocket == 0))
	{
		ASSERT_FAIELD;

		return FALSE;
	}

	SendDataNode _SendNode;

	_SendNode.bIsConnID  = false;

	_SendNode.u64ConnID   = hSocket;

	_SendNode.pPtr       = pDataBuffer;

	return m_SendDataList.Push(_SendNode);
}

BOOL CNetManager::CloseSendDataThread()
{
	m_bCloseSend = TRUE;

	SendDataNode _SendNode;

	m_SendDataList.Push(_SendNode);

	CommonThreadFunc::WaitThreadExit(m_hSendThread);

	return TRUE;
}

BOOL CNetManager::CloseEventThread()
{
	m_bCloseEvent = TRUE;

	for(std::vector<THANDLE>::iterator itor = m_vtEventThread.begin(); itor != m_vtEventThread.end(); ++itor)
	{
		CommonThreadFunc::WaitThreadExit(*itor);
	}

	return TRUE;
}

BOOL CNetManager::CloseDispatchThread()
{
	CommonThreadFunc::WaitThreadExit(m_hDispathThread);

	return TRUE;
}

Th_RetName _NetEventThread( void *pParam )
{
	CNetManager *pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_ProcessEvent();

	CLog::GetInstancePtr()->AddLog("网络事件处理线程退出!");

	return Th_RetValue;
}

Th_RetName _NetListenThread( void *pParam )
{
	CNetManager *pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_Listen();

	CLog::GetInstancePtr()->AddLog("监听线程退出!");

	return Th_RetValue;
}

Th_RetName _NetEventDispatchThread(void *pParam )
{
	CNetManager *pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_DispathEvent();

	CLog::GetInstancePtr()->AddLog("事件分发线程退出!");
	
	return Th_RetValue;
}

Th_RetName _NetSendBufferThread(void *pParam)
{
	CNetManager *pNetManager = CNetManager::GetInstancePtr();

	pNetManager->WorkThread_SendData();

	CLog::GetInstancePtr()->AddLog("数据发送线程退出!");

	return Th_RetValue;
}