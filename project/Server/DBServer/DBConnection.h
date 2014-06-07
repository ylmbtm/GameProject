
#ifndef _SQL_CONN_H_
#define _SQL_CONN_H_

#include <my_global.h>
#include <mysql.h>
#include <cstdio>

#define ERROR_SLEEP_TIME 3000 // 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

namespace CommonSql
{
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
    int Query( CDBStoredProcedure *pDBStoredProcedure, sql_result **result_ );

    // errno.
    int GetError( void ) const;

protected:
    // reconnect.
    bool Reconnect( void );

    // error process
    bool ProcError( char const *op_ = NULL, char const *func_ = NULL );

private:
    MYSQL       *m_pMySql;
	std::string   m_host_;
    std::string   m_user_;
	std::string   m_pwd_;
    std::string   m_db_;
    int          m_port_;
    int          m_errno_;
    std::string   m_error_;
};

} // namespace ex

#endif // _SQL_CONN_H_