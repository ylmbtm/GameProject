#pragma once
#ifndef __CLIENT_ENGINE_H__
#define __CLIENT_ENGINE_H__
#include "NetworkMgr.h"

struct IMessageHandler
{
	virtual BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper) = 0;
	//virtual BOOL OnConnection(bool bSuccessed);
};

class ClientEngine : public ICommandHandler
{
private:
	ClientEngine();

	~ClientEngine();

public:
	static ClientEngine* GetInstancePtr();

	BOOL InitEngine();

	BOOL CloseEngine();

	BOOL SetClientID(UINT64 u64ClientID);  //一般是角色ID

	BOOL ConnectToServer(std::string strIpAddr, UINT16 sPort);

	BOOL DisConnect();

	BOOL SendData(char *pData, UINT32 dwLen);

	IDataBuffer* GetWriteBuffer();

	BOOL Login(const char *pszAccountName, const char *pszPassword);

	BOOL RegisterMsgHandler(IMessageHandler *pMsgHandler);

	BOOL Render();

	//以下是内部的消息处理
	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick){return TRUE;};

	UINT32 OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdPickCharAck( UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper );
protected:

	UINT64				m_u64ClientID;

	CNetworkMgr			m_NetworkMgr;
	
	std::vector<IMessageHandler*> m_vtMsgHandler;

};

#endif
