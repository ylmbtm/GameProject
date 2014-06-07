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

	m_StoredProcedures[1] = new CDBStoredProcedure("CALL _DBServer_InsertUpdateRoleBlobInfo(?,?)", 2);
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

