
#include "sql_worker.h"

namespace ex
{

sql_worker::sql_worker( void )
{
}

sql_worker::~sql_worker( void )
{
    close();
}

// connect.
bool sql_worker::connect( char const *host_,
                          char const *user_,
                          char const *pwd_,
                          char const *db_,
                          int         port_ )
{
    return m_conn_.connect( host_, user_, pwd_, db_, port_ );
}

// close.
void sql_worker::close( void )
{
    map<size_t, sql_stmt*>::iterator _iter;
    for ( _iter = m_stmt_.begin();
          _iter != m_stmt_.end();
          ++ _iter )
    {
        if ( NULL != _iter->second )
        {
            _iter->second->release();
            _iter->second = NULL;
        }
    }
    m_stmt_.clear();
    m_conn_.close();
}

// add a prepared statement.
void sql_worker::add_stmt( size_t type_, char const *sql_ )
{
    if ( NULL != sql_ )
    {
        sql_stmt *_stmt = NULL;
        map<size_t, sql_stmt*>::iterator _iter;

        _stmt = m_conn_.create_stmt( sql_, strlen( sql_ ) );
        if ( NULL != _stmt )
        {
            _iter = m_stmt_.find( type_ );
            if ( _iter != m_stmt_.end() )
            {
                if ( NULL != _iter->second )
                {
                    _iter->second->release();
                    _iter->second = NULL;
                }
            }
            m_stmt_[type_] = _stmt;
        }
    }
}

// get a prepared statement by type.
sql_stmt* sql_worker::get_stmt( size_t type_ )
{
    sql_stmt *_ret = NULL;
    map<size_t, sql_stmt*>::iterator _iter;

    _iter = m_stmt_.find( type_ );
    if ( _iter != m_stmt_.end() )
    {
        _ret = _iter->second;
    }

    return _ret;
}

// execute.
int sql_worker::execute( sql_stmt *stmt_ )
{
    return m_conn_.execute( stmt_ );
}

// query.
int sql_worker::query( sql_stmt *stmt_, sql_result **result_ )
{
    return m_conn_.query( stmt_, result_ );
}

// errno.
int sql_worker::error( void ) const
{
    return m_conn_.error();
}

} // namepsace ex