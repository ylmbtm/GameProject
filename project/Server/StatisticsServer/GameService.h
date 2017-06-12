#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "StatCmdHandler.h"

class CGameService : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(CConnection *pConn);

	BOOL		OnCloseConnect(CConnection *pConn);

	BOOL		DispatchPacket( NetPacket *pNetPacket);

public:
	CServerCmdHandler   m_ServerCmdHandler;

	CStatCmdHandler		m_StatCmdHandler;

	//本服务器的信息
	UINT32              m_dwServerID;
	UINT32				m_dwServerType;
	std::string         m_strIpAddr;
	UINT16              m_sPort;

public:
	UINT32			GetServerID(){return m_dwServerID;}
	UINT32			GetServerType(){	return m_dwServerType;}
	std::string		GetCenterSvrIp() const { return m_strCenterSvrIp; }
	UINT16			GetCenterSvrPort() const { return m_sCenterSvrPort; }

	std::string     m_strCenterSvrIp;
	UINT16			m_sCenterSvrPort;
};

#endif
