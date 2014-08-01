#include "StdAfx.h"
#include "ClientEngine.h"
#include "CommandDef.h"
#include "DataBuffer\BufferHelper.h"
#include "ConnectionType.h"
#include "PacketDef\ClientPacket.h"
#include "Error.h"
#include "ObjectID.h"
#include "Utility\CommonSocket.h"

ClientEngine::ClientEngine(void)
{
	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	m_ConnectState	= Not_Connect;

	m_u64ClientID = 0;

	CommonSocket::InitNetwork();
}

ClientEngine::~ClientEngine(void)
{
	m_u64ClientID = 0;

	m_ConnectState	= Not_Connect;

	m_hSocket		= INVALID_SOCKET;

	m_nDataLen		= 0;

	CommonSocket::UninitNetwork();
}


BOOL ClientEngine::InitEngine()
{
	

	return TRUE;
}

BOOL ClientEngine::CloseEngine()
{

	return TRUE;
}

BOOL ClientEngine::SetClientID( UINT64 u64ClientID )
{
	m_u64ClientID = u64ClientID;

	return TRUE;
}

BOOL ClientEngine::SendData( char *pData, INT32 dwLen )
{
	if((pData == NULL)||(dwLen == 0))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

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

BOOL ClientEngine::Login(const char *pszAccountName, const char *pszPassword)
{
	if((pszPassword == NULL)||(pszAccountName == NULL))
	{
		return FALSE;
	}

	StCharLoginReq CharLoginReq;
	strncpy(CharLoginReq.szAccountName, pszAccountName, 32);
	strncpy(CharLoginReq.szPassword, pszPassword, 32);

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_CHAR_LOGIN_REQ, 0, 0, 0);

	WriteHelper.Write(CharLoginReq);

	WriteHelper.EndWrite();

	SendData(m_WriteBuffer.GetData(), m_WriteBuffer.GetDataLenth());

	return TRUE;
}

BOOL ClientEngine::RegisterMsgHandler(IMessageHandler *pMsgHandler)
{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		if(pMsgHandler == *itor)
		{
			return FALSE;
		}
	}
		
	m_vtMsgHandler.push_back(pMsgHandler);

	return TRUE;
}

BOOL ClientEngine::UnregisterMsgHandler( IMessageHandler *pMsgHandler )
{
	for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
	{
		if(pMsgHandler == *itor)
		{
			m_vtMsgHandler.erase(itor);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL ClientEngine::Render()
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

	if(ReceiveData())
	{
		ProcessData();
	}

	return TRUE;

}

BOOL ClientEngine::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CONNECT_NOTIFY,		OnCmdConnectNotify);

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_PICK_CHAR_ACK,	OnCmdPickCharAck);

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_HEART_BEAT_ACK,	OnCmdHearBeatAck);

	default:
		{
			for(std::vector<IMessageHandler*>::iterator itor = m_vtMsgHandler.begin(); itor != m_vtMsgHandler.end(); itor++)
			{
				IMessageHandler *pHandler = *itor;

				if(pHandler->OnCommandHandle(wCommandID, u64ConnID, pBufferHelper))
				{
					break;
				}
			}
		}
		break;
	}

	return TRUE;
}


BOOL ClientEngine::OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper)
{
	StConnectNotify ConnectNotify;

	pBufferHelper->Read(ConnectNotify);

	UINT32 ConType = ConnectNotify.btConType;

	ConnectNotify.btConType =  TYPE_CLT_PLAYER;

	ConnectNotify.u64ConnID = m_u64ClientID;

	CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

	WriteHelper.BeginWrite(CMD_CONNECT_NOTIFY, CMDH_SVR_CON, 0, 0);

	WriteHelper.Write(ConnectNotify);

	WriteHelper.EndWrite();

	SendData(m_WriteBuffer.GetData(), m_WriteBuffer.GetDataLenth());

	if(ConType == TYPE_SVR_PROXY)
	{
		StCharEnterGameReq CharEnterGameReq;

		CharEnterGameReq.u64CharID = m_u64ClientID;

		CharEnterGameReq.dwIdentifyCode = m_dwIdentifyCode;

		CHECK_PAYER_ID(m_u64ClientID);

		CBufferHelper WriteHelper(TRUE, &m_WriteBuffer);

		WriteHelper.BeginWrite(CMD_CHAR_ENTER_GAME_REQ, CMDH_SENCE, 0, CharEnterGameReq.u64CharID);

		WriteHelper.Write(CharEnterGameReq);

		WriteHelper.EndWrite();

		SendData(m_WriteBuffer.GetData(),m_WriteBuffer.GetDataLenth());
	}
	else if(ConType == TYPE_SVR_LOGIN)
	{
		SetConnectState(Succ_Connect);
	}

	return 0;
}

ClientEngine* ClientEngine::GetInstancePtr()
{
	static ClientEngine _Handler;

	return &_Handler;
}

IDataBuffer* ClientEngine::GetWriteBuffer()
{
	return &m_WriteBuffer;
}

BOOL ClientEngine::OnCmdPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharPickCharAck CharPickCharAck;
	pBufferHelper->Read(CharPickCharAck);

	CHECK_PAYER_ID(CharPickCharAck.u64CharID);

	if(CharPickCharAck.nRetCode == E_SUCCESSED)
	{
		DisConnect();
		m_u64ClientID = CharPickCharAck.u64CharID;
		m_dwIdentifyCode = CharPickCharAck.dwIdentifyCode;
		ConnectToServer(CharPickCharAck.szProxyIpAddr, CharPickCharAck.nProxyPort);
	}

	return TRUE;
}

BOOL ClientEngine::ConnectToServer( std::string strIpAddr, UINT16 sPort )
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

BOOL ClientEngine::DisConnect()
{
	CommonSocket::ShutDownRecv(m_hSocket);
	CommonSocket::ShutDownSend(m_hSocket);
	CommonSocket::CloseSocket(m_hSocket);

	m_ConnectState = Not_Connect;

	return TRUE;
}


void ClientEngine::SetConnectState( ConnectState val )
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


BOOL ClientEngine::ReceiveData()
{
	int nReadLen = recv(m_hSocket, m_DataBuffer + m_nDataLen, CONST_BUFF_SIZE - m_nDataLen, 0);
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

		return FALSE;
	}
	else if(nReadLen == 0)
	{
		printf("对方关闭了连接!\n");

		SetConnectState(Not_Connect); 

		CommonSocket::CloseSocket(m_hSocket);

		return FALSE;
	}
	else
	{
		m_nDataLen += nReadLen;
	}

	return TRUE;
}


BOOL ClientEngine::ProcessData()
{
	if(m_nDataLen < sizeof(TransferHeader))
	{
		return FALSE;
	}

	TransferHeader *pHeader = (TransferHeader *)m_DataBuffer;
	if(pHeader->dwSize <= m_nDataLen)
	{
		memcpy(m_ReadBuffer.GetData(), m_DataBuffer, pHeader->dwSize);

		m_ReadBuffer.SetDataLenth(pHeader->dwSize);

		m_nDataLen -= pHeader->dwSize;

		if(m_nDataLen > 0)
		{
			memmove(m_DataBuffer, m_DataBuffer+pHeader->dwSize, m_nDataLen);
		}

		CBufferHelper BufferReader(FALSE, &m_ReadBuffer);
		if(!BufferReader.BeginRead())
		{
			return FALSE;
		}

		CommandHeader *pCommandHeader = BufferReader.GetCommandHeader();
		if(pCommandHeader == NULL)
		{
			return FALSE;
		}

		OnCommandHandle(pCommandHeader->wCommandID, 0, &BufferReader);
	}

	return TRUE;
}

UINT32 ClientEngine::GetServerTime()
{
	UINT32 dwTick = ::GetTickCount();

	return m_dwServerTime + (m_dwServerTick - dwTick)/1000;
}

BOOL ClientEngine::OnCmdHearBeatAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharHeartBeatAck CharHeartBeatAck;
	pBufferHelper->Read(CharHeartBeatAck);

	UINT32 dwTick = ::GetTickCount();

	m_dwServerTime = CharHeartBeatAck.dwServerTime + (dwTick - CharHeartBeatAck.dwReqTimestamp) /1000;
	m_dwServerTick = dwTick;

	return TRUE;
}








