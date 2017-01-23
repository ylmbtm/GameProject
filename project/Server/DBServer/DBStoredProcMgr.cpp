#include "stdafx.h"
#include "DBStoredProcMgr.h"

CDBStoredProcedureMgr::CDBStoredProcedureMgr()
{

}

CDBStoredProcedureMgr::~CDBStoredProcedureMgr()
{

}


BOOL CDBStoredProcedureMgr::InitStoredProcedures()
{
	m_StoredProcedures.reserve(MAX_DBServerDB_STATEMENTS);

	m_StoredProcedures[DB_INSERT_PLAYER_INFO] = new CDBStoredProcedure("CALL InsertUpdateRoleBlobInfo(?,?)", 2);
	m_StoredProcedures[DB_INSERT_PLAYER_INFO]->m_DBRecordSet.SetFieldNum(6);
	m_StoredProcedures[DB_INSERT_PLAYER_INFO]->m_DBRecordSet.SetFieldType(0, MYSQL_TYPE_FLOAT);

	
	/*m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();
	m_StoredProcedures[] = new CDBStoredProcedure();*/

	return TRUE;
}

CDBStoredProcedure* CDBStoredProcedureMgr::GetStoredProcedure( DBStoredProcedureID SpID )
{
	if((SpID < 0)&&(SpID >= MAX_DBServerDB_STATEMENTS))
	{
		return NULL;
	}

	return m_StoredProcedures[SpID];
}

