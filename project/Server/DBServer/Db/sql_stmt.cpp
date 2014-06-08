
#include "sql_stmt.h"
#include "sql_conn.h"
#include "sql_result.h"
#include "Allocator.h"
#include <cstdio>
#include <errmsg.h>

namespace ex
{

sql_stmt::sql_stmt(  )
    : m_bind_( NULL ),
      m_count_( 0 )
{
}

sql_stmt::~sql_stmt( void )
{
    if ( NULL != m_bind_ )
    {
        free( m_bind_, m_count_ * sizeof( MYSQL_BIND ) );
        m_bind_  = NULL;
        m_count_ = 0;
    }
    m_params_.clear();
}

// close.
void sql_stmt::release( void )
{
    object_pool<sql_stmt>::free( this );
}

// set bool value.
void sql_stmt::set_bool( size_t idx_, my_bool bval_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char)bval_, 1 );
    _ptr->type = MYSQL_TYPE_TINY;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set int8 value.
void sql_stmt::set_int8( size_t idx_, int8 i8_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char)i8_, 1 );
    _ptr->type = MYSQL_TYPE_TINY;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set uint8 value.
void sql_stmt::set_uint8( size_t idx_, uint8 ui8_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char)ui8_, 1 );
    _ptr->type = MYSQL_TYPE_TINY;
    _ptr->is_unsigned = UNSIGNED_FLAG;
    _ptr->is_null_val = 0;
}

// set int16 value.
void sql_stmt::set_int16( size_t idx_, int16 i16_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&i16_, 2 );
    _ptr->type = MYSQL_TYPE_SHORT;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set uint16 value.
void sql_stmt::set_uint16( size_t idx_, uint16 ui16_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&ui16_, 2 );
    _ptr->type = MYSQL_TYPE_SHORT;
    _ptr->is_unsigned = UNSIGNED_FLAG;
    _ptr->is_null_val = 0;
}

// set int32 value.
void sql_stmt::set_int32( size_t idx_, int32 i32_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&i32_, 4 );
    _ptr->type = MYSQL_TYPE_LONG;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set uint32 value.
void sql_stmt::set_uint32( size_t idx_, uint32 ui32_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&ui32_, 4 );
    _ptr->type = MYSQL_TYPE_LONG;
    _ptr->is_unsigned = UNSIGNED_FLAG;
    _ptr->is_null_val = 0;
}

// set int64 value.
void sql_stmt::set_int64( size_t idx_, int64 i64_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&i64_, 8 );
    _ptr->type = MYSQL_TYPE_LONGLONG;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set uint64 value.
void sql_stmt::set_uint64( size_t idx_, uint64 ui64_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&ui64_, 8 );
    _ptr->type = MYSQL_TYPE_LONGLONG;
    _ptr->is_unsigned = UNSIGNED_FLAG;
    _ptr->is_null_val = 0;
}

// set float value.
void sql_stmt::set_float( size_t idx_, float fval_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&fval_, 4 );
    _ptr->type = MYSQL_TYPE_FLOAT;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set double value.
void sql_stmt::set_double( size_t idx_, double dval_ )
{
    param_t *_ptr = &m_params_[idx_];

    _ptr->buf.assign( (char*)&dval_, 8 );
    _ptr->type = MYSQL_TYPE_DOUBLE;
    _ptr->is_unsigned = 0;
    _ptr->is_null_val = 0;
}

// set string.
void sql_stmt::set_string( size_t idx_, char const *str_ )
{
    if ( NULL != str_ )
    {
        param_t *_ptr = &m_params_[idx_];

        _ptr->buf.assign( str_ );
        _ptr->type = MYSQL_TYPE_STRING;
        _ptr->is_unsigned = 0;
        _ptr->is_null_val = 0;
    }
}

// set string.
void sql_stmt::set_string( size_t idx_, char const *str_, size_t sz_ )
{
    if ( NULL != str_ )
    {
        param_t *_ptr = &m_params_[idx_];

        _ptr->buf.assign( str_, sz_ );
        _ptr->type = MYSQL_TYPE_STRING;
        _ptr->is_unsigned = 0;
        _ptr->is_null_val = 0;
    }
}

// set blob.
void sql_stmt::set_blob( size_t idx_, void const *ptr_, size_t sz_ )
{
    if ( NULL != ptr_ )
    {
        param_t *_ptr = &m_params_[idx_];

        _ptr->buf.assign( (char*)ptr_, sz_ );
        _ptr->type = MYSQL_TYPE_BLOB;
        _ptr->is_unsigned = 0;
        _ptr->is_null_val = 0;
    }
}

// set blob.
void sql_stmt::set_medium_blob( size_t idx_, void const *ptr_, size_t sz_ )
{
	if ( NULL != ptr_ )
	{
		param_t *_ptr = &m_params_[idx_];

		_ptr->buf.assign( (char*)ptr_, sz_ );
		_ptr->type = MYSQL_TYPE_MEDIUM_BLOB;
		_ptr->is_unsigned = 0;
		_ptr->is_null_val = 0;
	}
}

// bind parameters.
void sql_stmt::_bind( size_t count_ )
{
    if ( m_count_ < count_ )
    {
        if ( NULL != m_bind_ )
        {
            free( m_bind_, m_count_ * sizeof(MYSQL_BIND) );
            m_bind_ = NULL;
        }
        m_bind_ = (MYSQL_BIND*)malloc( count_ * sizeof(MYSQL_BIND) );
        m_count_ = count_;
    }

    if ( NULL != m_bind_ )
    {
        assert( count_ == m_params_.size() );
        if ( count_ == m_params_.size() )
        {
            param_t *_ptr = NULL;

            memset( m_bind_, 0, m_count_ * sizeof( MYSQL_BIND ) );
            for ( size_t _idx = 0; _idx < count_; ++ _idx )
            {
                _ptr                        = &m_params_[_idx];
                m_bind_[_idx].buffer        = (void*)_ptr->buf.c_str();
                m_bind_[_idx].buffer_length = _ptr->buf.size();
                m_bind_[_idx].buffer_type   = _ptr->type;
                m_bind_[_idx].is_unsigned   = _ptr->is_unsigned;
                m_bind_[_idx].is_null_value = _ptr->is_null_val;
            }
        }
        else
        {
            printf( "Error:[%s][%s][%d][parameter can't match].\n", m_sql_.c_str(), __FUNCTION__, __LINE__ );
        }
    }
}

} // namespace ex