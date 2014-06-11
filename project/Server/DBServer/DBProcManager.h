#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__

#include "Sqlite/CppSQLite3.h"
#include "PacketDef/ClientPacket.h"

class CDBProcManager
{
public:
	CDBProcManager(void);
	~CDBProcManager(void);

	BOOL InitManager();

	UINT32 GetMaxAccountID();

	//create new a account
	BOOL CreateAccount(char *szAccount, char *szPassword);

	//验证成功返回账号ID
	UINT32 VerifyAccount(char *szAccount, char *szPassword);

	//load account char info
	BOOL   LoadAccountCharInfo(UINT32 dwAccountID, StCharLoginAck &Ack);

	BOOL   CreateNewChar(StCharNewCharReq &Req, StCharNewCharAck &Ack);

	CppSQLite3DB	m_DBConnection;

	UINT32			m_dwMaxAccountID;
};
#endif