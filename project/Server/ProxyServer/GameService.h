#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "ServerCmdHandler.h"
#include "CharWillEnterList.h"
#include "ProxyCmdHandler.h"
class  CConnection;

class CGameService  : public IPacketDispatcher
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
	BOOL		OnCmdGMCommand(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL		OnCmdHeartBeatReq(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);


public:
	//处理普通的网络连接
	CServerCmdHandler   m_ServerCmdHandler;

	CProxyCmdHandler	m_ProxyCmdHandler;

	CWillEnterNodeMgr   m_WillEnterNodeMgr;

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

	void			SetDBConnID(UINT32 ConnID){m_dwDBConnID = ConnID;}
	BOOL			SendCmdToDBConnection(IDataBuffer *pDataBuf);

	void			SetWorldConnID( UINT32 ConnID ){m_dwWorldConnID = ConnID;}
	UINT32			GetWorldConnID() {return m_dwWorldConnID;}
protected:
	UINT32			m_dwDBConnID;
	UINT32			m_dwStatConnID;
	UINT32			m_dwWorldConnID;
	
};

#endif
