#include "stdafx.h"
#include "NetworkMgr.h"
#include "Utility/CommonSocket.h"
#include "resource.h"
#include "TestClientDlg.h"
#include "DataBuffer/DataBuffer.h"
#include "PacketHeader.h"
#include "DataBuffer/BufferHelper.h"
#include "CommandHandler.h"
#include "DataBuffer/BufferHelper.h"

CNetworkMgr::CNetworkMgr(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	m_bConnected	= FALSE;

	m_pReadBuffer	= new CDataBuffer<BUFFSIZE>;

	m_pWriteBuffer	= new CDataBuffer<BUFFSIZE>;
}

CNetworkMgr::~CNetworkMgr(void)
{
	if(m_pReadBuffer)
	{
		delete m_pReadBuffer;
	}

	if(m_pWriteBuffer)
	{
		delete m_pWriteBuffer;
	}

}

CNetworkMgr* CNetworkMgr::GetInstancePtr()
{
	static CNetworkMgr _NetworkMgr;

	return &_NetworkMgr;
}

BOOL CNetworkMgr::InitNetSystem()
{
	if(!CommonSocket::InitNetwork())
	{
		MessageBox(NULL, "网络初始化失败!", "错误提示", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkMgr::DisConnect()
{
	CommonSocket::CloseSocket(m_hSocket);

	m_bConnected = FALSE;

	return FALSE;
}

BOOL CNetworkMgr::ConnectToServer(std::string strIpAddr, UINT16 sPort)
{
	m_hSocket = CommonSocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
	if((m_hSocket == INVALID_SOCKET)||(m_hSocket == NULL))
	{
		MessageBox(NULL, "创建套接字失败!", "错误提示", MB_OK);
		return FALSE;
	}


	if(!CommonSocket::ConnectSocket(m_hSocket, strIpAddr.c_str(), sPort))
	{
		MessageBox(NULL, "连接服务器失败!", "错误提示", MB_OK);
		return FALSE;
	}
	
	CommonSocket::SetSocketUnblock(m_hSocket);

	return TRUE;
}

BOOL CNetworkMgr::OnTime()
{
	ReceiveData();

	ProcessData();

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
			//如果没有连接，表明连接成功
			if(!m_bConnected)
			{
				m_bConnected = TRUE;

				printf("连接成功1!");
			}

			if(nError != m_nLastError)
			{
				printf("接收数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

				m_nLastError = nError;
			}
		}
		else 
		{
			if(nError != m_nLastError)
			{
				printf("接收数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

				m_nLastError = nError;
			}

			if(m_bConnected)
			{
				m_bConnected = FALSE;
			}
		}
	}
	else if(nReadLen == 0)
	{
		printf("对方关闭了连接!");

		if(m_bConnected)
		{
			m_bConnected = FALSE;
		}
	}
	else
	{
		if(!m_bConnected)
		{
			m_bConnected = TRUE;

			printf("连接成功2!， 收到%d字节数据", nReadLen);
		}

		m_nDataLen += nReadLen;
	}
	

 	return TRUE;
}

BOOL CNetworkMgr::SendData( char *pData, UINT32 dwLen )
{
	if(!m_bConnected)
	{
		return FALSE;
	}

	int nWriteLen = send(m_hSocket,  (char*)pData, dwLen, 0);
	if(nWriteLen < 0)
	{
		DWORD nError = CommonSocket::GetSocketLastError();
		if(nError != m_nLastError)
		{
			printf("发送数据发生错误:%s!", CommonSocket::GetLastErrorStr(nError).c_str());

			m_nLastError = nError;
		}

		return FALSE;
	}
	else
	{
		if(nWriteLen < dwLen)
		{
			m_bConnected = FALSE;

			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CNetworkMgr::IsConnected()
{
	return m_bConnected;
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

		CClientCmdHandler::GetInstancePtr()->OnCommandHandle(pCommandHeader->wCommandID, 0, &BufferReader);
	}

	return TRUE;
}


