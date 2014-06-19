#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketDef/TransferPacket.h"
#include "Utility/CommonSocket.h"
#include "DataBuffer/BufferHelper.h"
#include "ConnectionType.h"
#include "GlobalConfig.h"
#include "CommandDef.h"
#include "Utility/Log/Log.h"

void NetIoOperatorData::Clear()
{
#ifdef WIN32
	memset(&Overlap, 0, sizeof(Overlap));

	dwCmdType = 0;

	pDataBuffer = NULL;
#endif
}


CConnection::CConnection(void)
{
	m_hSocket			= INVALID_SOCKET;

	m_u64ConnID			= 0;

	m_pDataHandler	= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_byteType			= TYPE_UNKNOW;
}

CConnection::~CConnection(void)
{
}

#ifdef WIN32

BOOL CConnection::DoReceive()
{
	WSABUF  DataBuf;

	DataBuf.len = CONST_BUFF_SIZE - m_dwDataLen;
	DataBuf.buf = m_pBuffer + m_dwDataLen;

	DWORD dwRecvBytes = 0, dwFlags = 0;

	m_IoOverlapRecv.Clear();
	m_IoOverlapRecv.dwCmdType = NET_CMD_RECV;

	int nRet = WSARecv(m_hSocket, &DataBuf, 1, &dwRecvBytes, &dwFlags, (LPOVERLAPPED)&m_IoOverlapRecv, NULL);
	if(nRet != 0)
	{
		//对于WSARecv来说， 只要返回0,就表示没有错误发生。
		//当返回为ERROR_IO_PENDING时，表示提交读数据请求成功， 其它的返回值都是错误。
		int nError = CommonSocket::GetSocketLastError();
		if(nError != ERROR_IO_PENDING )
		{
			CLog::GetInstancePtr()->AddLog("关闭连接，因为接收数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

			CloseConnection(TRUE);

			return FALSE;
		}
	}

	//对于WSARecv来说， 只要返回0,就表示没有错误发生。

	return TRUE;
}

#else

BOOL CConnection::DoReceive()
{
	while(TRUE)
	{
		int nBytes = recv(m_hSocket, m_pBuffer+m_dwDataLen, BUFF_SIZE - m_dwDataLen, 0);
		if(nBytes == 0)
		{
			if(m_dwDataLen == BUFF_SIZE)
			{
				CLog::GetInstancePtr()->AddLog("buffer满了，需要再读一次!!");

				ExtractBuffer();

				continue;
			}

			CLog::GetInstancePtr()->AddLog("收到数据为0， 判断是对方关闭################!!");

			CloseConnection(TRUE);

			return FALSE;
		}
		else if(nBytes < 0)
		{
			int nErr = CommonSocket::GetSocketLastError();
			if( nErr == EAGAIN)
			{
				CLog::GetInstancePtr()->AddLog("读成功了，缓冲区己经无数据可读!!");

				return TRUE;
			}
			else
			{
				CLog::GetInstancePtr()->AddLog("读失败， 可能连接己断开 原因:%s!!", CommonSocket::GetLastErrorStr(nErr).c_str());

				CloseConnection(TRUE);

				return FALSE;
			}
		}
		else
		{
			m_dwDataLen += nBytes;
		}
	}

	return TRUE;
}

#endif

UINT64 CConnection::GetConnectionID()
{
	return m_u64ConnID;
}

void CConnection::SetConnectionID( UINT64 u64ConnID )
{
	m_u64ConnID = u64ConnID;

	return ;
}

BOOL CConnection::ExtractBuffer()
{
	if(m_dwDataLen < sizeof(TransferHeader))
	{
		return FALSE;
	}

	IDataBuffer *pDataBuffer = NULL;

	TransferHeader *pHeader = (TransferHeader *)m_pBuffer;
	if(pHeader->dwSize <= m_dwDataLen)
	{
		pDataBuffer = CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pHeader->dwSize);

		memcpy(pDataBuffer->GetData(), m_pBuffer, pHeader->dwSize);

		m_dwDataLen -= pHeader->dwSize;

		if(m_dwDataLen > 0)
		{
			memmove(m_pBuffer, m_pBuffer+pHeader->dwSize, m_dwDataLen);
		}

		pDataBuffer->SetDataLenth(pHeader->dwSize);
	}

	if(pDataBuffer != NULL)
	{
		m_pDataHandler->OnDataHandle(pDataBuffer, this);
	}

	return TRUE;
}

BOOL CConnection::CloseConnection(BOOL bNotify)
{
	if((m_bConnected)&&(m_u64ConnID != 0))
	{
		m_pDataHandler->OnDisconnect(this);
	}

	m_bConnected = FALSE;

	CLog::GetInstancePtr()->AddLog("关闭连接 连接ID: %ld, Socket : %d!",m_u64ConnID, m_hSocket);

	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_hSocket = INVALID_SOCKET;

	CConnectionMgr::GetInstancePtr()->DeleteConnection(this);

	return TRUE;
}

BOOL CConnection::HandleRecvEvent(UINT32 dwBytes)
{
#ifdef WIN32
	m_dwDataLen += dwBytes;

	ExtractBuffer();

	DoReceive();
#else
	DoReceive();

	ExtractBuffer();
#endif
	return TRUE;
}

BOOL CConnection::SetSocket( SOCKET hSocket )
{
	m_hSocket = hSocket;

	return TRUE;
}

BOOL CConnection::SetDataHandler( IDataHandler *pHandler )
{
	if(pHandler == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	m_pDataHandler = pHandler;

	return TRUE;
}

SOCKET CConnection::GetSocket()
{
	return m_hSocket;
}

BOOL CConnection::IsConnectionOK()
{
	if((m_hSocket == INVALID_SOCKET)||(m_hSocket == 0))
	{
		return FALSE;
	}

	return m_bConnected;
}

BOOL CConnection::SetConnectionOK( BOOL bOk )
{
	m_bConnected = bOk;

	return TRUE;
}

UINT8 CConnection::GetConnectionType()
{
	return m_byteType;
}

VOID CConnection::SetConnectionType( UINT8 byteType )
{
	m_byteType = byteType;

	return ;
}

CConnectionMgr::CConnectionMgr()
{
	m_dwNextConnID = SVR_CONN_ID;

	for(int i = 0; i < SVR_CONN_ID; ++i)
	{
		m_StableConnList[i] = NULL;
	}
}

CConnectionMgr::~CConnectionMgr()
{

}

CConnection* CConnectionMgr::CreateConnection() 
{
	CConnection *pConnection = new CConnection();

	CAutoLock Lock(&m_CritSec);

	m_WaitConnList.insert(pConnection);

	return pConnection;
}

CConnection* CConnectionMgr::GetConnectionByConnID( UINT64 u64ConnID )
{
	if(u64ConnID < SVR_CONN_ID)
	{
		return m_StableConnList[u64ConnID];
	}
	else 
	{
		stdext::hash_map<UINT64, CConnection*>::iterator itor = m_VarieableConnList.find(u64ConnID);
		if(itor != m_VarieableConnList.end())
		{
			return itor->second;
		}
	}

	return NULL;
}

CConnectionMgr* CConnectionMgr::GetInstancePtr()
{
	static CConnectionMgr ConnectionMgr;

	return &ConnectionMgr;
}

BOOL CConnectionMgr::SetConnectionID(CConnection *pConnection, UINT64 u64ConnID)
{
	CAutoLock Lock(&m_CritSec);

	std::set<CConnection*>::iterator itor = m_WaitConnList.find(pConnection);
	if(itor != m_WaitConnList.end())
	{
		m_WaitConnList.erase(itor);
	}
	else
	{
		ASSERT_FAIELD;
	}

	if(u64ConnID == 0)
	{
		m_dwNextConnID++;
		if(m_dwNextConnID < SVR_CONN_ID)
		{
			m_dwNextConnID = SVR_CONN_ID+1;
		}
		
		pConnection->SetConnectionID(m_dwNextConnID);

		m_VarieableConnList.insert(std::make_pair(m_dwNextConnID, pConnection));
		
		return TRUE;
	}


	if(u64ConnID < SVR_CONN_ID)
	{
		ASSERT(m_StableConnList[u64ConnID] == NULL);
		pConnection->SetConnectionID(u64ConnID);
		m_StableConnList[u64ConnID] = pConnection;

		return TRUE;
	}

	if(u64ConnID > SVR_CONN_ID)
	{
		pConnection->SetConnectionID(u64ConnID);

		m_VarieableConnList.insert(std::make_pair(u64ConnID, pConnection));

		return TRUE;
	}

	return TRUE;
}

VOID CConnectionMgr::DeleteConnection( CConnection *pConnection )
{
	if(pConnection->GetConnectionID() == 0)
	{
		CAutoLock Lock(&m_CritSec);

		std::set<CConnection*>::iterator itor = m_WaitConnList.find(pConnection);
		if(itor != m_WaitConnList.end())
		{
			m_WaitConnList.erase(itor);
		}
	}
	else if(pConnection->GetConnectionID() < SVR_CONN_ID)
	{
		m_StableConnList[pConnection->GetConnectionID()] = NULL;
	}
	else
	{
		CAutoLock Lock(&m_CritSec);

		m_VarieableConnList.erase(pConnection->GetConnectionID());
	}
	
	delete pConnection;

	return ;
}

BOOL CConnectionMgr::CloseAllConnection()
{
	CConnection *pConn = NULL;
	for(int i = 0; i < SVR_CONN_ID; i++)
	{
		pConn = m_StableConnList[i];
		if(pConn != NULL)
		{
			pConn->CloseConnection(FALSE);
		}
	}

	for(std::set<CConnection*>::iterator itor = m_WaitConnList.begin(); itor != m_WaitConnList.end(); ++itor)
	{
		pConn = *itor;
		if(pConn != NULL)
		{
			pConn->CloseConnection(FALSE);
		}
	}

	for(stdext::hash_map<UINT64, CConnection*>::iterator itor = m_VarieableConnList.begin(); itor != m_VarieableConnList.end(); ++itor)
	{
		pConn = itor->second;
		if(pConn != NULL)
		{
			pConn->CloseConnection(FALSE);
		}
	}

	return TRUE;
}

BOOL CConnectionMgr::DestroyAllConnection()
{
	CConnection *pConn = NULL;
	for(int i = 0; i < SVR_CONN_ID; i++)
	{
		delete m_StableConnList[i];

		m_StableConnList[i] = NULL;
	}

	for(std::set<CConnection*>::iterator itor = m_WaitConnList.begin(); itor != m_WaitConnList.end(); ++itor)
	{
		pConn = *itor;

		delete pConn;
	}

	for(stdext::hash_map<UINT64, CConnection*>::iterator itor = m_VarieableConnList.begin(); itor != m_VarieableConnList.end(); ++itor)
	{
		pConn = itor->second;

		delete pConn;
	}

	 return TRUE;
}

