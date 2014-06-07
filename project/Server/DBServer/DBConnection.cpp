#include "stdafx.h"
#include <errmsg.h>
#include "DBConnection.h"
#include "sql_result.h"

namespace CommonSql
{
CDBConnection::CDBConnection( void )
    : m_mysql_( NULL ),
      m_port_( 3306 ),
      m_errno_( 0 ),
      m_error_( "" )
{
}

CDBConnection::~CDBConnection( void )
{
    Close();
}

// connect.
bool CDBConnection::Connect(char const *szHost, char const *szUser,char const *szPwd,char const *szDb,int nPort)
{
    Close();

    m_pMySql = mysql_init( NULL );
    if ( NULL != m_pMySql )
    {
        // set character.
        mysql_options( m_pMySql, MYSQL_SET_CHARSET_NAME, "utf8" );
        if ( NULL != mysql_real_connect( m_pMySql, szHost,szUser,szPwd, szDb, nPort,NULL,0 ) )
        {
            m_host_.assign( szHost );
            m_user_.assign( szUser );
            m_pwd_.assign( szPwd );
            m_db_.assign( szDb );
            m_port_ = nPort;

            // set autocommit.
            mysql_autocommit( m_pMySql, 1 );
            // set character set.
            mysql_set_character_set( m_pMySql, "utf8" );

            return true;
        }
        else
        {
            m_errno_ = mysql_errno( m_pMySql );
            m_error_ = mysql_error( m_pMySql );
        }
    }
    else
    {
        
    }

    return false;
}

// close.
void CDBConnection::Close( void )
{
    if ( NULL != m_pMySql )
    {
        mysql_close( m_pMySql );

        m_pMySql = NULL;
    }
}

// execute.
int CDBConnection::Execute(CDBStoredProcedure *pDBStoredProcedure)
{
    int nAffectedCount = -1;

	if((NULL == m_pMySql)||(pDBStoredProcedure == NULL))
	{
		return nAffectedCount;
	}

    MYSQL_STMT *pMySqlStmt = mysql_stmt_init(m_pMySql);
	if(pMySqlStmt == NULL)
	{
		m_errno_ = mysql_errno( m_pMySql );

		m_error_ = mysql_error( m_pMySql );
		
		return nAffectedCount;
	}

	if(0 != mysql_stmt_prepare(m_pMySql, pDBStoredProcedure->m_strSql.c_str(), pDBStoredProcedure->m_strSql.size()))
	{
		m_errno_ = mysql_errno( m_pMySql );

		m_error_ = mysql_error( m_pMySql );

		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;

		return nAffectedCount;
	}

    my_bool _bl = 1;
    mysql_stmt_attr_set( pMySqlStmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );

	//
	if(mysql_stmt_param_count( pMySqlStmt ) != pDBStoredProcedure->m_nCount)
	{
		m_errno_ = mysql_errno( m_pMySql );
		m_error_ = mysql_error( m_pMySql );

		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;

		return nAffectedCount;
	}

	if(pDBStoredProcedure->m_pMybind != NULL)
	{
		if ( 0 != mysql_stmt_bind_param( pMySqlStmt, pDBStoredProcedure->m_pMybind ) )
		{
			m_errno_ = mysql_errno( m_pMySql );
			m_error_ = mysql_error( m_pMySql );
			mysql_stmt_close( pMySqlStmt );
			pMySqlStmt = NULL;
			return nAffectedCount;
		}
	}

	if(pMySqlStmt == NULL)
	{
		m_errno_ = mysql_errno( m_pMySql );
		m_error_ = mysql_error( m_pMySql );
		return nAffectedCount;
	}

	 if (0 != mysql_stmt_execute( pMySqlStmt ))
	 {
		 m_errno_ = mysql_errno( m_pMySql );
		 m_error_ = mysql_error( m_pMySql );
		 mysql_stmt_close( pMySqlStmt );
		 pMySqlStmt = NULL;
		 return nAffectedCount;
	 }

    nAffectedCount = (int)mysql_stmt_affected_rows( pMySqlStmt );

    // 检查是否有结果集
    MYSQL_RES *pMySqlResult = mysql_stmt_result_metadata( pMySqlStmt );
	if(pMySqlResult == NULL)
	{
		pMySqlResult  = NULL;
		mysql_stmt_close( pMySqlStmt );
		pMySqlStmt = NULL;
		return nAffectedCount;
	}

    unsigned int server_status = m_pMySql->server_status;

    if ( server_status & SERVER_PS_OUT_PARAMS )
    {
        //write_log( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!\n" );
        mysql_stmt_store_result( pMySqlStmt );
        mysql_free_result( pMySqlResult );
        mysql_stmt_free_result( pMySqlStmt );
    }
    else
    {
        //write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
        mysql_stmt_store_result( pMySqlStmt );
        mysql_free_result( pMySqlResult );
        mysql_stmt_free_result( pMySqlStmt );

        while ( 0 == mysql_stmt_next_result( pMySqlStmt ) )
        {
            pMySqlResult = mysql_stmt_result_metadata( pMySqlStmt );
            if ( NULL != pMySqlResult )
            {
                server_status = m_mysql_->server_status;
                mysql_stmt_store_result( pMySqlStmt );
                mysql_free_result( pMySqlResult );
                mysql_stmt_free_result( pMySqlStmt );
                if ( server_status & SERVER_PS_OUT_PARAMS )
                {
                    //write_log( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!\n" );
                    break;
                }
                else
                {
                   // write_log( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!\n" );
                }
            }
        }
    }
    
    return nAffectedCount;
}

// query.
int CDBConnection::Query( CDBStoredProcedure *pDBStoredProcedure, sql_result **result_ )
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
bool CDBConnection::Reconnect( void )
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
bool CDBConnection::ProcError( char const *op_/* = NULL*/, char const *func_/* = NULL*/ )
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
int CDBConnection::GetError( void ) const
{
    return m_errno_;
}

} // namespace ex