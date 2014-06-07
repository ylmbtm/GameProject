#ifndef __SERVICE_BASE_H__
#define __SERVICE_BASE_H__

#include "GlobalConfig.h"
#include "IBufferHandler.h"
#include "Connection.h"

class ServiceBase : public IDataHandler, 
					public ICommandHandler
{
public:
	ServiceBase(void);

	virtual ~ServiceBase(void);
	
	BOOL			StartService();

	BOOL			StopService();

	BOOL			OnDataHandle( IDataBuffer *pDataBuffer , CConnection *pConnection);

	BOOL			OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL			OnUpdate(UINT32 dwTick);

	BOOL			OnDisconnect(CConnection *pConnection);

public:
	BOOL			ConnectToOtherSvr(std::string strIpAddr, UINT16 sPort);

	BOOL			SendCmdToConnection(UINT64 u64ConnID, IDataBuffer *pSrcBuffer);

	BOOL			SendCmdToConnection(UINT64 u64ConnID, UINT64 u64CharID, UINT32 dwSceneID, IDataBuffer *pSrcBuffer );

	CConnection*    GetConnectionByID(UINT64 u64ConnID);

public:


};


#endif /*__SERVICE_BASE_H__*/