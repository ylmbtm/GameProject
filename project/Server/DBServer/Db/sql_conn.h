
#ifndef _SQL_CONN_H_
#define _SQL_CONN_H_

#include <my_global.h>
#include <mysql.h>
#include <cstdio>
#include "sql_string.h"

#define ERROR_SLEEP_TIME 3000 // 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

namespace ex
{

class sql_stmt;
class sql_result;

class sql_conn
{
public:
    sql_conn( void );
    ~sql_conn( void );

    // connect.
    bool connect( char const *host_,
                  char const *user_,
                  char const *pwd_,
                  char const *db_,
                  int         port_ );

    // close.
    void close( void );

    // create statement.
    sql_stmt* create_stmt( char const *sql_, size_t sz_ );

    // execute.
    int execute( sql_stmt *stmt_ );

    // query.
    int query( sql_stmt *stmt_, sql_result **result_ );

    // errno.
    int error( void ) const;

protected:
    // reconnect.
    bool _reconnect( void );

    // process error.
    bool _procerror( char const *op_ = NULL, char const *func_ = NULL );

private:
    MYSQL       *m_mysql_;
    sql_string   m_host_;
    sql_string   m_user_;
    sql_string   m_pwd_;
    sql_string   m_db_;
    int          m_port_;
    int          m_errno_;
    sql_string   m_error_;
};

} // namespace ex

#endif // _SQL_CONN_H_