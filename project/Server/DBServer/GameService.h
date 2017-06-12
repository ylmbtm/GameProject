#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "DBCmdHandler.h"
class CConnection;

#define DB_THREAD_NUM 10

class CGameService : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

public:
	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(CConnection *pConn);

	BOOL		OnCloseConnect(CConnection *pConn);

	BOOL		DispatchPacket( NetPacket *pNetPacket);

public:
	CServerCmdHandler   m_ServerCmdHandler;


	CDBCmdHandler		m_DBCmdHandler[DB_THREAD_NUM];

public:
	//本服务器的信息
	UINT32              m_dwServerID;
	UINT32				m_dwServerType;
	std::string         m_strIpAddr;
	UINT16              m_sPort;
	std::string			m_strCenterSvrIp;
	UINT16				m_sCenterSvrPort;

	std::string		GetCenterSvrIp() const { return m_strCenterSvrIp; }
	UINT16			GetCenterSvrPort() const { return m_sCenterSvrPort; }

	UINT32			GetServerID(){return m_dwServerID;}
	UINT32			GetServerType(){	return m_dwServerType;}
public:
	void			SetStatConnID(UINT32 ConnID){m_dwStatConnID = ConnID;}
	BOOL			SendCmdToStatConnection(IDataBuffer *pDataBuf);

	void			SetWorldConnID( UINT32 ConnID ){m_dwWorldConnID = ConnID;}
	UINT32			GetWorldConnID() {return m_dwWorldConnID;}
protected:
	UINT32			m_dwStatConnID;
	UINT32			m_dwWorldConnID;
};

#endif
