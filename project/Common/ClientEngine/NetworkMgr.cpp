#include "stdafx.h"
#include "NetworkMgr.h"
#include "PacketHeader.h"
#include "Utility/CommonSocket.h"
#include "DataBuffer/DataBuffer.h"
#include "DataBuffer/BufferHelper.h"

CNetworkMgr::CNetworkMgr(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	m_ConnectState	= Not_Connect;

	m_pReadBuffer	= new CDataBuffer<BUFFSIZE>;

	m_pWriteBuffer	= new CDataBuffer<BUFFSIZE>;

	CommonSocket::InitNetwork();
}

CNetworkMgr::~CNetworkMgr(void)
{
	CommonSocket::UninitNetwork();

	if(m_pReadBuffer)
	{
		delete m_pReadBuffer;
	}

	if(m_pWriteBuffer)
	{
		delete m_pWriteBuffer;
	}

}

BOOL CNetworkMgr::DisConnect()
{
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_ConnectState = Not_Connect;

	return FALSE;
}

BOOL CNetworkMgr::ConnectToServer(std::string strIpAddr, UINT16 sPort)
{
	SetConnectState(Start_Connect);

	m_hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if((m_hSocket == INVALID_SOCKET)||(m_hSocket == NULL))
	{
		printf("创建套接字失败!\n");

		SetConnectState(Not_Connect);

		return FALSE;
	}


	if(!CommonSocket::ConnectSocket(m_hSocket, strIpAddr.c_str(), sPort))
	{
		printf("连接服务器失败!\n");

		SetConnectState(Not_Connect);

		CommonSocket::CloseSocket(m_hSocket);

		return FALSE;
	}
	
	CommonSocket::SetSocketUnblock(m_hSocket);

	SetConnectState(Raw_Connect);

	return TRUE;
}

BOOL CNetworkMgr::ReceiveData()
{
	int nReadLen = recv(m_hSocket, m_Buffer + m_nDataLen, BUFFSIZE - m_nDataLen, 0);
	if(nReadLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();
		if(nError == WSAEWOULDBLOCK)
		{

		}
		else 
		{
			printf("接收数据发生错误:%s!\n", CommonSocket::GetLastErrorStr(nError).c_str());
		}
	}
	else if(nReadLen == 0)
	{
		printf("对方关闭了连接!\n");

		SetConnectState(Not_Connect); 

		CommonSocket::CloseSocket(m_hSocket);
	}
	else
	{
		m_nDataLen += nReadLen;
	}
	

 	return TRUE;
}

BOOL CNetworkMgr::SendData( char *pData, UINT32 dwLen )
{
	if((m_ConnectState != Succ_Connect)&&(m_ConnectState != Raw_Connect))
	{
		printf("没有连接服务器!\n");

		return FALSE;
	}

	int nWriteLen = send(m_hSocket,  (char*)pData, dwLen, 0);
	if(nWriteLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();

		printf("发送数据发生错误:%s!\n", CommonSocket::GetLastErrorStr(nError).c_str());

		return FALSE;
	}
	else
	{
		if(nWriteLen < dwLen)
		{
			SetConnectState(Not_Connect);
			CommonSocket::CloseSocket(m_hSocket);
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CNetworkMgr::ProcessData()
{
	if(m_nDataLen < sizeof(TransferHeader))
	{
		return FALSE;
	}
	
	TransferHeader *pHeader = (TransferHeader *)m_Buffer;
	if(pHeader->dwSize <= m_nDataLen)
	{
		memcpy(m_pReadBuffer->GetData(), m_Buffer, pHeader->dwSize);
		
		m_pReadBuffer->SetDataLenth(pHeader->dwSize);

		m_nDataLen -= pHeader->dwSize;

		if(m_nDataLen > 0)
		{
			memmove(m_Buffer, m_Buffer+pHeader->dwSize, m_nDataLen);
		}

		CBufferHelper BufferReader(FALSE, m_pReadBuffer);
		if(!BufferReader.BeginRead())
		{
			return FALSE;
		}

		CommandHeader *pCommandHeader = BufferReader.GetCommandHeader();
		if(pCommandHeader == NULL)
		{
			return FALSE;
		}

		m_EngineMsgHandler->OnCommandHandle(pCommandHeader->wCommandID, 0, &BufferReader);
	}

	return TRUE;
}

BOOL CNetworkMgr::ProcessOnce()
{
	if(m_ConnectState == Not_Connect)
	{
		return FALSE;
	}

	if(m_ConnectState == Start_Connect)
	{
		return FALSE;
	}

	if(m_ConnectState == Raw_Connect)
	{

	}

	ReceiveData();

	ProcessData();

	return TRUE;
}

void CNetworkMgr::SetConnectState( ConnectState val )
{
	if(val == Not_Connect)
	{
		printf("设置未连接!\n");
	}
	else if(val == Start_Connect)
	{
		printf("设置开始连接!\n");
	}
	else if(val == Raw_Connect)
	{
		printf("设置己完成原始连接!\n");
	}
	else if(val == Succ_Connect)
	{
		printf("设置连接成功!\n");
	}
	
	m_ConnectState = val;
}


