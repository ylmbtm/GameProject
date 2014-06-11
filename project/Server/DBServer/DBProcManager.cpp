#include "stdafx.h"
#include "DBProcManager.h"
#include "Utility\CommonFunc.h"


CDBProcManager::CDBProcManager(void)
{
}

CDBProcManager::~CDBProcManager(void)
{
}

BOOL CDBProcManager::InitManager()
{
	std::string strCurDir = CommonFunc::GetCurrentDir();
	strCurDir+= "\\GameData.db";

	m_DBConnection.open(strCurDir.c_str());

	return TRUE;
}

BOOL CDBProcManager::CreateAccount( char *szAccount, char *szPassword )
{
	CHAR szSql[MAX_PATH];

	sprintf(szSql, "insert into t_accountinfo(F_AccountName, F_Password) values('%s', '%s')", szAccount, szPassword);

	if(m_DBConnection.execDML(szSql) > 0)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CDBProcManager::LoadAccountCharInfo( UINT32 dwAccountID, StCharLoginAck &Ack )
{
	CHAR szSql[MAX_PATH];

	sprintf(szSql, "select * from t_charinfo where F_AccountID = '%d'", dwAccountID);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	int i = 0;
	
	while(!QueryRes.eof())
	{
		Ack.CharPickInfo[i].dwFeature = QueryRes.getIntField("F_Feature", 0);
		strncpy(Ack.CharPickInfo[i].szCharName, QueryRes.getStringField("F_Name", ""), 32);
		Ack.CharPickInfo[i].dwLevel = QueryRes.getIntField("F_Level", 0);
		i++;
		QueryRes.nextRow();
	}

	Ack.nCount = i;

	return TRUE;
}

UINT32 CDBProcManager::VerifyAccount( char *szAccount, char *szPassword )
{
	CHAR szSql[MAX_PATH];

	sprintf(szSql, "select * from t_accountinfo where F_AccountName ='%s' AND F_Password='%s'", szAccount, szPassword);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	while(!QueryRes.eof())  
	{  
		return QueryRes.getIntField("F_AccountID", 0);
		QueryRes.nextRow();
	}  
	
	return 0;
}

BOOL CDBProcManager::CreateNewChar(StCharNewCharReq &Req,  StCharNewCharAck &Ack)
{
	CHAR szSql[MAX_PATH];

	sprintf(szSql, "insert into t_charinfo(F_AccountID, F_Name, F_Feature) values('%d', '%s','%s')", Req.dwAccountID, Req.szCharName, Req.dwFeature);

	if(m_DBConnection.execDML(szSql) <= 0)
	{
		return FALSE;
	}
	
	sprintf(szSql, "select * from t_charinfo where F_Name ='%s'", Req.szCharName);

	CppSQLite3Query QueryRes = m_DBConnection.execQuery(szSql);

	QueryRes.nextRow();
	while(!QueryRes.eof())
	{
		Ack.CharPickInfo.dwFeature = QueryRes.getIntField("F_Feature", 0);
		strncpy(Ack.CharPickInfo.szCharName, QueryRes.getStringField("F_Name", ""), 32);
		Ack.CharPickInfo.dwLevel = QueryRes.getIntField("F_Level", 0);
	}

	return TRUE;
}

UINT32 CDBProcManager::GetMaxAccountID()
{

	return 0;
}
