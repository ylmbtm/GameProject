
#ifndef _SQL_CONN_H_
#define _SQL_CONN_H_

#include <my_global.h>
#include <mysql.h>
#include <cstdio>

#ifdef _DEBUG
	#pragma comment(lib, "libmysqld.lib")
#else
	#pragma comment(lib, "libmysql.lib")
#endif

#define ERROR_SLEEP_TIME 3000 // 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

class CDBStoredProcedure;
class sql_result;

class CDBConnection
{
public:
    CDBConnection( void );
    ~CDBConnection( void );

    // connect.
    bool Connect(char const *szHost, char const *szUser,char const *szPwd,char const *szDb,int nPort);

    // close.
    void Close( void );

    // execute.
    int Execute( CDBStoredProcedure *pDBStoredProcedure );

    // query.
    int Query( CDBStoredProcedure *pDBStoredProcedure);

    // errno.
    int GetError( void ) const;

protected:
    // reconnect.
    bool Reconnect( void );

    // error process
    bool ProcError( char const *op_ = NULL, char const *func_ = NULL );

private:
    MYSQL       *m_pMySql;
	std::string  m_strHost;
    std::string  m_strUser;
	std::string  m_strPwd;
    std::string  m_strDB;
    int          m_nPort;
    int          m_nErrno;
    std::string  m_strError;
};

#define INSERT_QUERY "INSERT INTO bintest(id, data) VALUES(4, ?)"

bool ExeProcedure(MYSQL *sql, int nProcedureID, ...)
{
	
}

bool insertBlob(MYSQL* sql, char * blobData, unsigned long size)
{
	do
	{
		MYSQL_BIND bind[1] = {0};
		MYSQL_STMT *stmt = mysql_stmt_init(sql);  
		if (NULL == stmt)
			break;  
		if (mysql_stmt_prepare(stmt, INSERT_QUERY, strlen(INSERT_QUERY)))
			break;
		bind[0].buffer      = blobData;
		bind[0].buffer_type = MYSQL_TYPE_BLOB;
		bind[0].length      = &size;
		bind[0].is_null     = 0;
		if (mysql_stmt_bind_param(stmt, bind))
			break;
		if (mysql_stmt_execute(stmt))
			break;
		return true ;
	} while(0);
	return false ;
}

#endif // _SQL_CONN_H_