#ifndef __CLIENT_CMD_HANDLER_H__
#define __CLIENT_CMD_HANDLER_H__

#include "IBufferHandler.h"
#include "CommandDef.h"
#include "PlayerObject.h"
#include "DlgSelect.h"

class CClientCmdHandler : public ICommandHandler
{
private:
	CClientCmdHandler(void);

	~CClientCmdHandler(void);

public:
	static CClientCmdHandler* GetInstancePtr();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick);


	BOOL SendLoginReq(LPCTSTR szAccountName, LPCTSTR szPassword);
	BOOL SendNewAccountReq(LPCTSTR szAccountName, LPCTSTR szPassword);
	BOOL SendNewCharReq(UINT32 dwAccountID,LPCTSTR szCharName, UINT32 dwFeature);
	BOOL SendPickCharReq(UINT64 u64CharID);


	//*********************消息处理定义开始******************************
public:
	UINT32 OnCmdConnectNotify(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdLoginGameAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdEnterGameAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	UINT32 OnCmdNewAccountAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdNewCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdPickCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);


	//CMD_CHAR_NEARBY_ADD,			//添加周围的对象
	//CMD_CHAR_NEARBY_UPDATE,		//更新周围的对象
	//CMD_CHAR_NEARBY_REMOVE,		//删除周围的对象

	UINT32 OnCmdNearByAdd(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdNearByUpdate(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	UINT32 OnCmdNearByRemove(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	//*********************消息处理定义结束******************************

	CPlayerObject m_HostPlayer;

	CPlayerObjectMgr m_PlayerObjMgr;

	CDlgSelect DlgSelect;
};


#endif