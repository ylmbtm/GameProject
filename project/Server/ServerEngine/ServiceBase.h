#ifndef __SERVICE_BASE_H__
#define __SERVICE_BASE_H__

#include "IBufferHandler.h"
#include "Connection.h"
#include "EventFuncManager.h"
#include "Utility/IniFile/ConfigFile.h"

struct NetPacket
{
	NetPacket( CConnection *pConnect = NULL, IDataBuffer *pBuffer = NULL, UINT32 dwCmdID = 0 )
	{
		m_pConnect = pConnect;

		m_pDataBuffer = pBuffer;

		m_dwCmdID = dwCmdID;
	}

	CConnection *m_pConnect;

	IDataBuffer *m_pDataBuffer;

	UINT32       m_dwCmdID;
};

class ServiceBase : public IDataHandler//, public CEventFuncManager
{
protected:
	ServiceBase(void);
	virtual ~ServiceBase(void);
public:
	static ServiceBase* GetInstancePtr();
	
    BOOL            StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher *pDispather);

    BOOL            StopNetwork();

	BOOL			OnDataHandle(IDataBuffer *pDataBuffer , CConnection *pConnection);

	BOOL			OnCloseConnect(CConnection *pConnection);

	BOOL			OnNewConnect(CConnection *pConnection);

	CConnection*	ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	BOOL			SendCmdToConnection(UINT32 dwConnID, IDataBuffer *pSrcBuffer);

	BOOL			SendCmdToConnection(UINT32 dwConnID, UINT64 u64CharID, UINT32 dwSceneID, IDataBuffer *pSrcBuffer );

	template<typename T>
	BOOL			SendCmdToConnection(UINT16 uCmdID, T &Data, UINT32 dwConnID, UINT64 uCharID = 0,UINT32 dwSceneID = 0);

	CConnection* GetConnectionByID(UINT32 dwConnID);
protected:
	IPacketDispatcher				  *m_pPacketDispatcher;
};


#endif /*__SERVICE_BASE_H__*/