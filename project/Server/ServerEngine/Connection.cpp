#include "stdafx.h"
#include "Connection.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketDef/ClientPacket.h"
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

    m_dwConnID            = 0;

	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_byteType			= TYPE_UNKNOW;

    m_pNext = NULL;
}

CConnection::~CConnection(void)
{
    m_hSocket            = INVALID_SOCKET;

	m_pDataHandler		= NULL;

	m_dwDataLen			= 0;

	m_bConnected		= FALSE;

	m_byteType			= TYPE_UNKNOW;

    m_pNext = NULL;
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
		int nBytes = recv(m_hSocket, m_pBuffer+m_dwDataLen, CONST_BUFF_SIZE - m_dwDataLen, 0);
		if(nBytes == 0)
		{
			if(m_dwDataLen == CONST_BUFF_SIZE)
			{
				CLog::GetInstancePtr()->AddLog("buffer满了，需要再读一次!!");

				while (ExtractBuffer());
				
				continue;
			}

			CLog::GetInstancePtr()->AddLog("收到数据为0， 判断是对方关闭################!!");

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

                return FALSE;
            }
        }
        else
        {
            m_dwDataLen += nBytes;

            while (ExtractBuffer());

            continue;
        }
    }

	return TRUE;
}

#endif

UINT32 CConnection::GetConnectionID()
{
    return m_dwConnID;
}

void CConnection::SetConnectionID( UINT32 dwConnID )
{
    ASSERT(m_dwConnID == 0);
    ASSERT(dwConnID != 0);
    ASSERT(!m_bConnected);

    m_dwConnID = dwConnID;

	return ;
}

BOOL CConnection::ExtractBuffer()
{
	if(m_dwDataLen < sizeof(PacketHeader))
	{
		return FALSE;
	}

	CheckPacketHeader();

    PacketHeader *pHeader = (PacketHeader *)m_pBuffer;
    if(pHeader->dwSize > m_dwDataLen)
    {
        return FALSE;
    }

    IDataBuffer *pDataBuffer =  CBufferManagerAll::GetInstancePtr()->AllocDataBuff(pHeader->dwSize);

	memcpy(pDataBuffer->GetBuffer(), m_pBuffer, pHeader->dwSize);

	m_dwDataLen -= pHeader->dwSize;

	if(m_dwDataLen > 0)
	{
		memmove(m_pBuffer, m_pBuffer+pHeader->dwSize, m_dwDataLen);
	}

    pDataBuffer->SetTotalLenth(pHeader->dwSize);

    m_pDataHandler->OnDataHandle(pDataBuffer, this);

    return TRUE;
}

BOOL CConnection::Close(BOOL bNotify)
{
    if((m_bConnected)&&bNotify)
    {
        m_pDataHandler->OnDisconnect(this);
    }

	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

    m_hSocket           = INVALID_SOCKET;
    m_bConnected       = FALSE;
    m_dwDataLen     = 0;

	return TRUE;
}

BOOL CConnection::HandleRecvEvent(UINT32 dwBytes)
{
#ifdef WIN32
	m_dwDataLen += dwBytes;

	while (ExtractBuffer());

	if (!DoReceive())
	{
		return FALSE;
	}
#else
	if (!DoReceive())
	{
		return FALSE;
	}

	while (ExtractBuffer());
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

BOOL CConnection::ReInit()
{
     m_hSocket = INVALID_SOCKET;

    m_bConnected = FALSE;

     m_byteType  = TYPE_UNKNOW;

    m_pDataHandler = NULL;

    m_dwDataLen = 0;

    m_dwIpAddr  = 0;

    return TRUE;
}

BOOL CConnection::CheckPacketHeader()
{

	return TRUE;
}

CConnectionMgr::CConnectionMgr()
{
    m_pFreeConnRoot = NULL;
}

CConnectionMgr::~CConnectionMgr()
{

}

CConnection* CConnectionMgr::CreateConnection() 
{
    CAutoLock Lock(&m_CritSec);

    if(m_pFreeConnRoot == NULL)
    {
        ASSERT_FAIELD;
        return NULL;
    }

    CConnection* pTemp = m_pFreeConnRoot;

    m_pFreeConnRoot = pTemp->m_pNext;

    if(pTemp->GetConnectionID() == 0)
    {
        ASSERT_FAIELD;
        return NULL;
    }

    return pTemp;
}

CConnection* CConnectionMgr::GetConnectionByConnID( UINT32 dwConnID )
{
    if(dwConnID >= m_vtConnList.size())
    {
        ASSERT_FAIELD;
        return NULL;
    }

    return &m_vtConnList.at(dwConnID);
}

SOCKET CConnectionMgr::GetConnectionSocket( UINT32 dwConnID )
{
    CAutoLock Lock(&m_CritSec);
    CConnection *pConnection = GetConnectionByConnID(dwConnID);
    if(pConnection == NULL)
    {
        return INVALID_SOCKET;
    }

	if(!pConnection->IsConnectionOK())
	{
		return INVALID_SOCKET;
	}

	return pConnection->GetSocket();
}


CConnectionMgr* CConnectionMgr::GetInstancePtr()
{
	static CConnectionMgr ConnectionMgr;

	return &ConnectionMgr;
}


VOID CConnectionMgr::DeleteConnection( CConnection *pConnection )
{
    CAutoLock Lock(&m_CritSec);
    
    if(pConnection == NULL)
    {
        ASSERT_FAIELD;
        return ;
    }

    pConnection->m_pNext = m_pFreeConnRoot;

    m_pFreeConnRoot = pConnection;

	return ;
}

BOOL CConnectionMgr::CloseAllConnection()
{
    CConnection *pConn = NULL;
    for(size_t i = 0; i < m_vtConnList.size(); i++)
    {
            CConnection &Conn = m_vtConnList.at(i);
            Conn.Close(FALSE);
    }

    return TRUE;
}

BOOL CConnectionMgr::DestroyAllConnection()
{
    m_vtConnList.clear();

	return TRUE;
}

BOOL CConnectionMgr::InitConnectionList(UINT32 nMaxCons)
{
    ASSERT(m_pFreeConnRoot == NULL);

    CConnection *pTemp = NULL;

    m_vtConnList.assign(nMaxCons+1, CConnection());
    for(UINT32 i = 1; i < nMaxCons+1; i++)
    {
        CConnection &conn = m_vtConnList.at(i);

        conn.SetConnectionID(i) ;

        if (m_pFreeConnRoot = NULL)
        {
            m_pFreeConnRoot = &conn;
            pTemp = m_pFreeConnRoot;
        }
        else
        {
            pTemp->m_pNext = &conn;
            pTemp = pTemp->m_pNext;
        }
    }

	 return TRUE;
}


