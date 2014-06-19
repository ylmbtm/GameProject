#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "ServiceBase.h"
#include "SceneManager.h"
#include "ServerCmdHandler.h"

class CGameService :
	public ServiceBase
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	BOOL		OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL		OnDisconnect(CConnection *pConnection);

	BOOL		StartRun();

	BOOL		OnIdle();

	BOOL		LoadScene();

public:
	UINT64				m_u64DBConnID;
	UINT64				GetDBConnID() const;
	void				SetDBConnID(UINT64 ConnID);
	BOOL				SendCmdToDBConnection(IDataBuffer *pDataBuf);

protected:
	CSceneManager m_SceneManager;

	CServerCmdHandler m_ServerCmdHandler;

};

#endif
