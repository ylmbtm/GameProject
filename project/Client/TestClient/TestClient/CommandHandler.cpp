#include "stdafx.h"
#include "CommandHandler.h"
#include "PacketDef/ServerPacket.h"
#include "PacketDef/TransferPacket.h"
#include "DataBuffer/BufferHelper.h"
#include "ConnectionType.h"
#include "PacketDef/ClientPacket.h"
#include "resource.h"
#include "TestClientDlg.h"
#include "DataBuffer/BufferHelper.h"
#include "PacketDef/LoginPacket.h"
#include "DlgSelect.h"
#include "Error.h"

CClientCmdHandler::CClientCmdHandler(void)
{
	ClientEngine::GetInstancePtr()->RegisterMsgHandler((IMessageHandler*)this);
}

CClientCmdHandler::~CClientCmdHandler(void)
{
}

CClientCmdHandler* CClientCmdHandler::GetInstancePtr()
{
	static CClientCmdHandler _Handler;

	return &_Handler;
}

BOOL CClientCmdHandler::OnCommandHandle( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	switch(wCommandID)
	{
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_LOGIN_ACK,		OnCmdLoginGameAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEW_CHAR_ACK,	OnCmdNewCharAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEW_ACCOUNT_ACK,OnCmdNewAccountAck);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_ENTER_GAME_ACK,	OnCmdEnterGameAck)

		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_ADD,		OnCmdNearByAdd);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_UPDATE,	OnCmdNearByUpdate);
		PROCESS_COMMAND_ITEM_T(CMD_CHAR_NEARBY_REMOVE,	OnCmdNearByRemove);

		;

	default:
		{

		}
		break;
	}

	return TRUE;
}

UINT32 CClientCmdHandler::OnCmdNearByAdd( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	printf("BEGIN---添加角色消息，添加人数:%d", dwCount);
	
	for(UINT32 i = 0; i < dwCount; i++)
	{
		CString strText;

		UINT8 type = 0;

		pBufferHelper->Read(type);

		if(type != 1)
		{
			continue;
		}

		CPlayerObject *pObject = new CPlayerObject;

		pObject->ReadFromBuffer(pBufferHelper);

		m_PlayerObjMgr.insert(std::make_pair(pObject->GetObjectID(), pObject));

		printf("添加角色:%d, 坐标x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);

	}

	printf("END---添加角色消息");


	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdNearByUpdate( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObject = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObject != NULL)
		{
			pObject->ReadFromBuffer(pBufferHelper);

			printf("更新角色:%d, 坐标x = %f, z = %f", (UINT32)pObject->GetObjectID(), pObject->m_ObjectPos.x, pObject->m_ObjectPos.z);
		}
		else
		{
			ASSERT_FAIELD;
		}
	}

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdNearByRemove( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	UINT32 dwCount = 0;
	pBufferHelper->Read(dwCount);

	printf("BEGIN---删除角色消息，预计删除人数:%d, 现在人数:%d", dwCount, m_PlayerObjMgr.size());

	for(UINT32 i = 0; i < dwCount; i++)
	{
		UINT64 u64CharID = 0;

		pBufferHelper->Read(u64CharID);

		CPlayerObject *pObj = m_PlayerObjMgr.GetPlayer(u64CharID);
		if(pObj == NULL)
		{
			ASSERT_FAIELD;
			return 0;
		}

		m_PlayerObjMgr.RemovePlayer(u64CharID);

		delete pObj;

		printf("删除角色:%d成功", (UINT32)u64CharID);

	}

	printf("END---删除角色消息");


	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

UINT32 CClientCmdHandler::OnCmdEnterGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharEnterGameAck CharEnterGameAck;

	pBufferHelper->Read(CharEnterGameAck);

	m_HostPlayer.ReadFromBuffer(pBufferHelper);

	printf("登录成功!");

	((CTestClientDlg*)AfxGetMainWnd())->m_DlgGame.Invalidate();

	return 0;
}

BOOL CClientCmdHandler::OnUpdate( UINT32 dwTick )
{

	return TRUE;
}


BOOL CClientCmdHandler::SendNewAccountReq( LPCTSTR szAccountName, LPCTSTR szPassword )
{
	StCharNewAccountReq CharNewAccountReq;
	strncpy(CharNewAccountReq.szAccountName, szAccountName, 32);
	strncpy(CharNewAccountReq.szPassword, szPassword, 32);

	CBufferHelper WriteHelper(TRUE, ClientEngine::GetInstancePtr()->GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_NEW_ACCOUNT_REQ, 0, 0, 0);

	WriteHelper.Write(CharNewAccountReq);

	WriteHelper.EndWrite();

	ClientEngine::GetInstancePtr()->SendData(ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetData(), ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetDataLenth());

	return TRUE;
}



UINT32 CClientCmdHandler::OnCmdLoginGameAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharLoginAck MsgLoginAck;

	pBufferHelper->Read(MsgLoginAck);

	if(MsgLoginAck.nRetCode == E_FAILED)
	{
		MessageBox(NULL, "登录失败! 密码或账号不对!!","提示", MB_OK);
	}
	else
	{
		DlgSelect.m_dwAccountID = MsgLoginAck.dwAccountID;
		DlgSelect.m_nCount = MsgLoginAck.nCount;
		for(int i = 0; i < MsgLoginAck.nCount; i++)
		{
			DlgSelect.m_CharInfoList[i] = MsgLoginAck.CharPickInfo[i];
		}
		
		DlgSelect.DoModal();
	}

	return TRUE;
}


BOOL CClientCmdHandler::SendPickCharReq( UINT64 u64CharID )
{
	StCharPickCharReq CharPickCharReq;
	CharPickCharReq.u64CharID = u64CharID;

	CBufferHelper WriteHelper(TRUE, ClientEngine::GetInstancePtr()->GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_PICK_CHAR_REQ, 0, 0, 0);

	WriteHelper.Write(CharPickCharReq);

	WriteHelper.EndWrite();

	ClientEngine::GetInstancePtr()->SendData(ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetData(), ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetDataLenth());

	return TRUE;
}


UINT32 CClientCmdHandler::OnCmdNewAccountAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewAccountAck CharNewAccountAck;
	pBufferHelper->Read(CharNewAccountAck);
	if(CharNewAccountAck.nRetCode == E_SUCCESSED)
	{
		MessageBox(NULL,"注册账号成功!", "提示", MB_OK);
	}
	else
	{
		MessageBox(NULL,"注册账号失败!", "提示", MB_OK);
	}

	return 0;
}

BOOL CClientCmdHandler::SendNewCharReq( UINT32 dwAccountID , LPCTSTR szCharName, UINT32 dwFeature)
{
	StCharNewCharReq CharNewCharReq;
	CharNewCharReq.dwFeature = dwFeature;
	CharNewCharReq.dwAccountID = dwAccountID;
	strncpy(CharNewCharReq.szCharName, szCharName, 32);
	CBufferHelper WriteHelper(TRUE, ClientEngine::GetInstancePtr()->GetWriteBuffer());

	WriteHelper.BeginWrite(CMD_CHAR_NEW_CHAR_REQ, 0, 0, 0);

	WriteHelper.Write(CharNewCharReq);

	WriteHelper.EndWrite();

	ClientEngine::GetInstancePtr()->SendData(ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetData(), ClientEngine::GetInstancePtr()->GetWriteBuffer()->GetDataLenth());

	return TRUE;
}


UINT32 CClientCmdHandler::OnCmdNewCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper )
{
	StCharNewCharAck CharNewCharAck;
	pBufferHelper->Read(CharNewCharAck);


	DlgSelect.AddCharPickInfo(CharNewCharAck.CharPickInfo);

	DlgSelect.DoModal();
	return 0;
}




