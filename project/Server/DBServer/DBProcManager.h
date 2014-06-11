#pragma once

class CDBProcManager
{
public:
	CDBProcManager(void);
	~CDBProcManager(void);

	BOOL InitManager();

	BOOL CreateAccount(char *szAccount, char *szPassword);
};
