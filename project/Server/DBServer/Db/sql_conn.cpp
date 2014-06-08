
#include <errmsg.h>
#include "sql_conn.h"
#include "sql_stmt.h"
#include "sql_result.h"

namespace ex
{

inline void write_log( char const *fmt_, ... )
{
    static bool _check = false;
    static char _name[256] = {'\0'};

    if ( '\0' == _name[0] )
    {
        CreateDirectoryA( "db_logs", NULL );
        sprintf( _name, "db_logs/db_mysql_(pid=%d).log", GetCurrentProcessId() );
    }

    // check.
    if ( !_check )
    {
      struct stat _st;

      _check = true;
      if ( 0 == stat( _name, &_st ) && _st.st_size > ( 1 << 20 ) ) // 1MB
      {
        ::remove( _name );
      }
    }

    FILE *_file = ::fopen( _name, "a" );

    if ( NULL != _file )
    {
        time_t _time = time( NULL );
        struct tm *_tm = localtime( &_time );
        char _tmp[1024] = "";
        va_list vargs;

        int _sz = sprintf( _tmp,
                           "%04d/%02d/%02d-%02d:%02d:%02d - ",
                           _tm->tm_year + 1900,
                           _tm->tm_mon + 1,
                           _tm->tm_mday,
                           _tm->tm_hour,
                           _tm->tm_min,
                           _tm->tm_sec );

        va_start( vargs, fmt_ );
        _sz = vsprintf( _tmp + _sz, fmt_, vargs );
        va_end( vargs );
        fputs( _tmp, _file );
        fclose( _file );
    }
}

sql_conn::sql_conn( void )
    : m_mysql_( NULL ),
      m_port_( 3306 ),
      m_errno_( 0 ),
      m_error_( "" )
{
}

sql_conn::~sql_conn( void )
{
    close();
}

// connect.
bool sql_conn::connect( char const *host_,
                        char const *user_,
                        char const *pwd_,
                        char const *db_,
                        int         port_ )
{
    close();

    m_mysql_ = mysql_init( NULL );

    if ( NULL != m_mysql_ )
    {
        // set character.
        mysql_options( m_mysql_, MYSQL_SET_CHARSET_NAME, "utf8" );
        if ( NULL != mysql_real_connect( m_mysql_,
                                         host_,
                                         user_,
                                         pwd_,
                                         db_,
                                         port_,
                                         NULL,
                                         0 ) )
        {
            m_host_.assign( host_ );
            m_user_.assign( user_ );
            m_pwd_.assign( pwd_ );
            m_db_.assign( db_ );
            m_port_ = port_;

            // set autocommit.
            mysql_autocommit( m_mysql_, 1 );
            // set character set.
            mysql_set_character_set( m_mysql_, "utf8" );

            write_log( "connect mysql server succeed!\n" );
            write_log( "mysql client library: %s.\n", mysql_get_client_info() );
            write_log( "mysql server version: %s.\n", mysql_get_server_info( m_mysql_ ) );

            return true;
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );
            write_log( "connect mysql server failed, errno = %d!\n", m_errno_ );
        }
    }
    else
    {
        write_log( "connect mysql server failed!\n" );
    }

    return false;
}

// close.
void sql_conn::close( void )
{
    if ( NULL != m_mysql_ )
    {
        mysql_close( m_mysql_ );
        m_mysql_ = NULL;
    }
}

sql_stmt* sql_conn::create_stmt( char const *sql_, size_t sz_ )
{
    sql_stmt *_ret = NULL;

    if ( NULL != m_mysql_ && NULL != sql_ && sz_ > 0 )
    {
        _ret = object_pool<sql_stmt>::malloc();
        _ret->m_sql_.assign( sql_, sz_ );
    }

    return _ret;
}

// execute.
int sql_conn::execute( sql_stmt *stmt_ )
{
    int _ret = -1;

    if ( NULL != m_mysql_ && NULL != stmt_ )
    {
        MYSQL_STMT *_stmt = mysql_stmt_init( m_mysql_ );

        if ( NULL != _stmt )
        {
            if ( 0 == mysql_stmt_prepare( _stmt, stmt_->m_sql_.c_str(), stmt_->m_sql_.size() ) )
            {
                my_bool _bl = 1;

                mysql_stmt_attr_set( _stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );
                stmt_->_bind( mysql_stmt_param_count( _stmt ) );

                if ( NULL != stmt_->m_bind_ )
                {
                    if ( 0 != mysql_stmt_bind_param( _stmt, stmt_->m_bind_ ) )
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_bind_param" ) )
                        {
                            _ret = execute( stmt_ );
                        }
                    }
                }

                if ( NULL != _stmt )
                {
                    if ( 0 == mysql_stmt_execute( _stmt ) )
                    {
                        _ret = (int)mysql_stmt_affected_rows( _stmt );

                        // 检查是否有结果集
                        MYSQL_RES *_res = mysql_stmt_result_metadata( _stmt );
                        if ( NULL != _res )
                        {
                            unsigned int server_status = m_mysql_->server_status;

                            if ( server_status & SERVER_PS_OUT_PARAMS )
                            {
                                write_log( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!\n" );
                                mysql_stmt_store_result( _stmt );
                                mysql_free_result( _res );
                                mysql_stmt_free_result( _stmt );
                            }
                            else
                            {
                                write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
                                mysql_stmt_store_result( _stmt );
                                mysql_free_result( _res );
                                mysql_stmt_free_result( _stmt );

                                while ( 0 == mysql_stmt_next_result( _stmt ) )
                                {
                                    _res = mysql_stmt_result_metadata( _stmt );
                                    if ( NULL != _res )
                                    {
                                        server_status = m_mysql_->server_status;
                                        mysql_stmt_store_result( _stmt );
                                        mysql_free_result( _res );
                                        mysql_stmt_free_result( _stmt );
                                        if ( server_status & SERVER_PS_OUT_PARAMS )
                                        {
                                            write_log( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!\n" );
                                            break;
                                        }
                                        else
                                        {
                                            write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
                                        }
                                    }
                                }
                            }
                        }
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        _res  = NULL;
                    }
                    else
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_execute" ) )
                        {
                            _ret = execute( stmt_ );
                        }
                    }
                }
            }
            else
            {
                m_errno_ = mysql_errno( m_mysql_ );
                m_error_ = mysql_error( m_mysql_ );
                mysql_stmt_close( _stmt );
                _stmt = NULL;
                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_prepare" ) )
                {
                    _ret = execute( stmt_ );
                }
            }
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );
            if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_init" ) )
            {
                _ret = execute( stmt_ );
            }
        }
    }

    return _ret;
}

// query.
int sql_conn::query( sql_stmt *stmt_, sql_result **result_ )
{
    int _ret = -1;

    if ( NULL != m_mysql_ && NULL != stmt_ && NULL != result_ )
    {
        MYSQL_STMT *_stmt = mysql_stmt_init( m_mysql_ );

        *result_ = NULL;
        if ( NULL != _stmt )
        {
            if ( 0 == mysql_stmt_prepare( _stmt, stmt_->m_sql_.c_str(), stmt_->m_sql_.size() ) )
            {
                my_bool _bl = 1;

                mysql_stmt_attr_set( _stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );
                stmt_->_bind( mysql_stmt_param_count( _stmt ) );

                if ( NULL != stmt_->m_bind_ )
                {
                    if ( 0 != mysql_stmt_bind_param( _stmt, stmt_->m_bind_ ) )
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_bind_param" ) )
                        {
                            _ret = query( stmt_, result_ );
                        }
                    }
                }

                if ( NULL != _stmt )
                {
                    if ( 0 == mysql_stmt_execute( _stmt ) )
                    {
                        _ret = (int)mysql_stmt_affected_rows( _stmt );
                        MYSQL_RES *_res = mysql_stmt_result_metadata( _stmt );
                        if ( NULL != _res )
                        {
                            unsigned int server_status = m_mysql_->server_status;

                            if ( 0 == mysql_stmt_store_result( _stmt ) )
                            {
                                sql_result *_result = NULL;

                                _result = object_pool<sql_result>::malloc();
                                *result_ = _result;
                                if ( server_status & SERVER_PS_OUT_PARAMS )
                                {
                                    _ret = 0;
                                    _result->_retval( _stmt, _res );
                                    mysql_free_result( _res );
                                    mysql_stmt_free_result( _stmt );
                                }
                                else
                                {
                                    _ret = (int)mysql_stmt_num_rows( _stmt );
                                    _result->_init( _stmt, _res );
                                    mysql_free_result( _res );
                                    mysql_stmt_free_result( _stmt );

                                    // 检查是否还有结果集
                                    while ( 0 == mysql_stmt_next_result( _stmt ) )
                                    {
                                        _res = mysql_stmt_result_metadata( _stmt );
                                        if ( NULL != _res )
                                        {
                                            server_status = m_mysql_->server_status;
                                            if ( 0 == mysql_stmt_store_result( _stmt ) )
                                            {
                                                if ( server_status & SERVER_PS_OUT_PARAMS )
                                                {
                                                    _result->_retval( _stmt, _res );
                                                    mysql_free_result( _res );
                                                    mysql_stmt_free_result( _stmt );
                                                    break;
                                                }
                                                else
                                                {
                                                    write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
                                                    mysql_stmt_store_result( _stmt );
                                                    mysql_free_result( _res );
                                                    mysql_stmt_free_result( _stmt );
                                                }
                                            }
                                        }
                                    }
                                }
                                mysql_stmt_close( _stmt );
                                _stmt = NULL;
                            }
                            else
                            {
                                m_errno_ = mysql_errno( m_mysql_ );
                                m_error_ = mysql_error( m_mysql_ );
                                mysql_stmt_close( _stmt );
                                _stmt = NULL;
                                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_store_result" ) )
                                {
                                    _ret = query( stmt_, result_ );
                                }
                                else
                                {
                                    _ret = -1;
                                }
                            }
                        }
                        else
                        {
                            mysql_stmt_close( _stmt );
                            _stmt = NULL;
                        }
                    }
                    else
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_execute" ) )
                        {
                            _ret = query( stmt_, result_ );
                        }
                    }
                }
            }
            else
            {
                m_errno_ = mysql_errno( m_mysql_ );
                m_error_ = mysql_error( m_mysql_ );
                mysql_stmt_close( _stmt );
                _stmt = NULL;
                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_prepare" ) )
                {
                    _ret = query( stmt_, result_ );
                }
            }
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );
            if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_init" ) )
            {
                _ret = query( stmt_, result_ );
            }
        }
    }

    return _ret;
}

// reconnect.
bool sql_conn::_reconnect( void )
{
    if ( NULL != m_mysql_ && 0 == mysql_ping( m_mysql_ ) )
    {
        return true;
    }
    else
    {
        write_log( "try to reconnect mysql server...\n" );

        close();
        m_mysql_ = mysql_init( NULL );
        if ( NULL != m_mysql_ )
        {
            // set character.
            mysql_options( m_mysql_, MYSQL_SET_CHARSET_NAME, "utf8" );
            if ( NULL != mysql_real_connect( m_mysql_,
                                             m_host_,
                                             m_user_,
                                             m_pwd_,
                                             m_db_,
                                             m_port_,
                                             NULL,
                                             0 ) )
            {
                // set autocommit.
                mysql_autocommit( m_mysql_, 1 );
                // set character set.
                mysql_set_character_set( m_mysql_, "utf8" );

                write_log( "reconnect mysql server succeed!\n" );
                write_log( "mysql client library: %s.\n", mysql_get_client_info() );
                write_log( "mysql server version: %s.\n", mysql_get_server_info( m_mysql_ ) );

                return true;
            }
            else
            {
                write_log( "reconnect mysql server failed, errno = %d!\n", mysql_errno( m_mysql_ ) );
            }
        }
        else
        {
            write_log( "reconnect mysql server failed!" );
        }

    }

    return false;
}

// process error.
bool sql_conn::_procerror( char const *op_/* = NULL*/, char const *func_/* = NULL*/ )
{
    bool _ret = false;

    if ( NULL != op_ && NULL != func_ )
    {
        write_log( "op = %s, func = %s.\n", op_, func_ );
    }
    else if ( NULL != op_ )
    {
        write_log( "op = %s.\n", op_ );
    }

    switch ( m_errno_ )
    {
    case CR_SERVER_GONE_ERROR:
        {
            write_log( "mysql server has gone away, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
					::Sleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_SERVER_LOST:
        {
            write_log( "lost the connection to mysql server, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    Sleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_INVALID_CONN_HANDLE:
        {
            write_log( "invalid connection handle, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    Sleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_SERVER_LOST_EXTENDED:
        {
            write_log( "lost the connection to mysql server, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    Sleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    default:
        {
            write_log( "%s, errno = %d!\n", m_error_.c_str(), m_errno_ );
        }
        break;
    }

    return _ret;
}

// errno.
int sql_conn::error( void ) const
{
    return m_errno_;
}

} // namespace ex