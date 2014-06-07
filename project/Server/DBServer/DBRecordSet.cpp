/*
**  copyright (c) 2012 - all rights reserved.
**
**  proj:   mysql
**  file:   sql_result.cpp
**  author: hello.will.1990@gmail.com
**  date:   2012/12/11
**  comment:
*/
#include "stdafx.h"
#include "DBRecordSet.h"
#include <cstdio>

CDBRecordSet::CDBRecordSet( void )
{

}

CDBRecordSet::~CDBRecordSet( void )
{
}

bool CDBRecordSet::MoveNext( void )
{
    bool _ret = false;

    if ( 0 == m_row_ &&
         m_row_ < m_res_.size() &&
         m_cur_ != m_res_.end() )
    {
        ++ m_row_;
        _ret = true;
    }
    else
    {
        if ( m_cur_ != m_res_.end() )
        {
            ++ m_cur_;
            if ( m_cur_ != m_res_.end() )
            {
                ++ m_row_;
                _ret = true;
            }
        }
    }

    return _ret;
}

size_t CDBRecordSet::GetCount( void ) const
{
	return m_res_.size();
}

my_bool CDBRecordSet::get_bool( size_t idx_ )
{
    bool _ret = false;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = ( 0 != ( *(uint8*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = ( 0 != ( *(uint16*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_INT24:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONG:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = ( 0 != ( *(uint64*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = ( 0 != ( *(uint64*)_res->buf.c_str() ) );
                break;
            }
        }
    }

    return _ret;
}

int8 CDBRecordSet::get_int8( size_t idx_ )
{
    int8 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int8)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int8)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int8)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int8)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int8)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint8 CDBRecordSet::get_uint8( size_t idx_ )
{
    uint8 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint8)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint8)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint8)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint8)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint8)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int16 CDBRecordSet::get_int16( size_t idx_ )
{
    int16 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int16)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int16)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int16)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int16)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int16)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint16 CDBRecordSet::get_uint16( size_t idx_ )
{
    uint16 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint16)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint16)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint16)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint16)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint16)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int32 CDBRecordSet::get_int32( size_t idx_ )
{
    int32 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int32)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int32)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int32)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int32)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int32)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint32 CDBRecordSet::get_uint32( size_t idx_ )
{
    uint32 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint32)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint32)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint32)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint32)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint32)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int64 CDBRecordSet::get_int64( size_t idx_ )
{
    int64 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int64)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int64)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int64)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int64)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int64)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint64 CDBRecordSet::get_uint64( size_t idx_ )
{
    uint64 _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint64)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint64)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint64)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint64)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint64)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

float CDBRecordSet::get_float( size_t idx_ )
{
    float _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (float)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (float)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (float)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (float)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (float)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (float)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (float)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

double CDBRecordSet::get_double( size_t idx_ )
{
    double _ret = 0;

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (double)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (double)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (double)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (double)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (double)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (double)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (double)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

std::pair<size_t, char const*> CDBRecordSet::get_string( size_t idx_ )
{
    std::pair<size_t, char const*> _ret( 0, "" );

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            _ret.first  = _res->size;
            _ret.second = _res->buf.c_str();
            *(((char*)_ret.second) + _ret.first) = '\0';
        }
    }

    return _ret;
}

std::pair<size_t, void const*> CDBRecordSet::get_blob( size_t idx_ )
{
    std::pair<size_t, void const*> _ret( 0, "" );

    if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t *_res = &(*m_cur_)[idx_];

        if ( !_res->is_null )
        {
            _ret.first  = _res->size;
            _ret.second = _res->buf.c_str();
        }
    }

    return _ret;
}

// get medium blob.
std::pair<size_t, void const*> CDBRecordSet::get_medium_blob( size_t idx_ )
{
	std::pair<size_t, void const*> _ret( 0, "" );

	if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
	{
		result_t *_res = &(*m_cur_)[idx_];

		if ( !_res->is_null )
		{
			_ret.first  = _res->size;
			_ret.second = _res->buf.c_str();
		}
	}

	return _ret;
}

my_bool CDBRecordSet::getr_bool( size_t idx_ )
{
    bool _ret = false;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = ( 0 != ( *(uint8*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = ( 0 != ( *(uint16*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_INT24:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONG:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = ( 0 != ( *(uint64*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = ( 0 != ( *(uint32*)_res->buf.c_str() ) );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = ( 0 != ( *(uint64*)_res->buf.c_str() ) );
                break;
            }
        }
    }

    return _ret;
}

int8 CDBRecordSet::getr_int8( size_t idx_ )
{
    int8 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int8)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int8)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int8)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int8)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int8)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint8 CDBRecordSet::getr_uint8( size_t idx_ )
{
    uint8 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint8)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint8)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint8)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint8)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint8)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint8)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int16 CDBRecordSet::getr_int16( size_t idx_ )
{
    int16 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int16)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int16)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int16)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int16)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int16)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint16 CDBRecordSet::getr_uint16( size_t idx_ )
{
    uint16 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint16)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint16)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint16)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint16)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint16)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint16)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int32 CDBRecordSet::getr_int32( size_t idx_ )
{
    int32 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int32)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int32)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int32)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int32)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int32)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint32 CDBRecordSet::getr_uint32( size_t idx_ )
{
    uint32 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint32)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint32)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint32)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint32)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint32)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint32)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

int64 CDBRecordSet::getr_int64( size_t idx_ )
{
    int64 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (int64)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (int64)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (int64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (int64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (int64)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (int64)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (int64)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

uint64 CDBRecordSet::getr_uint64( size_t idx_ )
{
    uint64 _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint64)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint64)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint64)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint64)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint64)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint64)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

float CDBRecordSet::getr_float( size_t idx_ )
{
    float _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (float)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (float)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (float)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (float)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (float)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (float)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (float)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

double CDBRecordSet::getr_double( size_t idx_ )
{
    double _ret = 0;

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            switch ( _res->type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (double)( *(uint8*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (double)( *(uint16*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (double)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (double)( *(uint32*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (double)( *(uint64*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (double)( *(float*)_res->buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (double)( *(double*)_res->buf.c_str() );
                break;
            }
        }
    }

    return _ret;
}

std::pair<size_t, char const*> CDBRecordSet::getr_string( size_t idx_ )
{
    std::pair<size_t, char const*> _ret( 0, "" );

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            _ret.first  = _res->size;
            _ret.second = _res->buf.c_str();
            *(((char*)_ret.second) + _ret.first) = '\0';
        }
    }

    return _ret;
}

std::pair<size_t, void const*> CDBRecordSet::getr_blob( size_t idx_ )
{
    std::pair<size_t, void const*> _ret( 0, "" );

    if ( idx_ < m_ret_.size() )
    {
        result_t *_res = &m_ret_[idx_];

        if ( !_res->is_null )
        {
            _ret.first  = _res->size;
            _ret.second = _res->buf.c_str();
        }
    }

    return _ret;
}

bool CDBRecordSet::_init( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
{
    bool _ret = false;

    if ( NULL != stmt_ && NULL != res_ )
    {
        size_t _row = 0;

        _row = mysql_stmt_num_rows( stmt_ );
        if ( _row > 0 )
        {
            size_t _count = 0;
            MYSQL_BIND *_bind = NULL;

            _count = mysql_stmt_field_count( stmt_ );
            if ( _count > 0 )
            {
                _bind = (MYSQL_BIND*)malloc( _count * sizeof(MYSQL_BIND) );
                memset( _bind, 0, _count * sizeof(MYSQL_BIND) );
            }

            if ( NULL != _bind )
            {
                map<size_t, result_t> _result;
                size_t _idx = 0;
                MYSQL_FIELD *_field = NULL;

                _field = mysql_fetch_field( res_ );
                while ( NULL != _field )
                {
                    size_t    _sz  = 0;

                    switch ( _field->type )
                    {
                    case MYSQL_TYPE_TINY:
                        _sz = 1;
                        break;
                    case MYSQL_TYPE_SHORT:
                        _sz = 2;
                        break;
                    case MYSQL_TYPE_INT24:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONG:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONGLONG:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_FLOAT:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_DOUBLE:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_BLOB:
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                        _sz = _field->max_length;
                        break;
                    }

                    result_t *_res          = &_result[_idx];
                    _res->buf.assign( '\0', _sz );
                    _res->size              = _sz;
                    _res->type              = _field->type;
                    _res->is_null           = 0;

                    _bind[_idx].buffer_type   = _field->type;
                    _bind[_idx].buffer        = (void*)_res->buf.c_str();
                    _bind[_idx].buffer_length = _res->size;
                    _bind[_idx].length        = &_res->size;
                    _bind[_idx].is_unsigned   = _field->flags & UNSIGNED_FLAG;
                    _bind[_idx].is_null       = &_res->is_null;

                    ++ _idx;
                    _field = mysql_fetch_field( res_ );
                }

                if ( 0 == mysql_stmt_bind_result( stmt_, _bind ) )
                {
                    while ( _row > 0 )
                    {
                        int _val = mysql_stmt_fetch( stmt_ );

                        -- _row;
                        if ( 0 == _val || MYSQL_DATA_TRUNCATED == _val )
                        {
                            result_t *_src = NULL;
                            result_t *_dest = NULL;

                            m_res_.push_back( map<size_t, result_t>() );
                            map<size_t, result_t> &_res = m_res_.back();
                            for ( size_t _idx = 0; _idx < _count; ++ _idx )
                            {
                                _src           = &_result[_idx];
                                _dest          = &_res[_idx];
                                _dest->buf     = _src->buf;
                                _dest->size    = _src->size;
                                _dest->type    = _src->type;
                                _dest->is_null = _src->is_null;
                            }
                        }
                        else
                        {
                            _row = 0;
                        }
                    }

                    m_cur_ = m_res_.begin();
                    m_row_ = 0;
                    _ret   = true;
                }

                free( _bind, _count * sizeof(MYSQL_BIND) );
                _result.clear();
            }
        }
    }

    return _ret;
}

bool CDBRecordSet::_retval( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
{
    bool _ret = false;

    if ( NULL != stmt_ && NULL != res_ )
    {
        size_t _row = 0;

        _row = mysql_stmt_num_rows( stmt_ );
        if ( 1 == _row )    // 输出参数返回的结果集只有一行
        {
            size_t _count = 0;
            MYSQL_BIND *_bind = NULL;

            _count = mysql_stmt_field_count( stmt_ );
            if ( _count > 0 )
            {
                _bind = (MYSQL_BIND*)malloc( _count * sizeof(MYSQL_BIND) );
                memset( _bind, 0, _count * sizeof(MYSQL_BIND) );
            }

            if ( NULL != _bind )
            {
                map<size_t, result_t> _result;
                size_t _idx = 0;
                MYSQL_FIELD *_field = NULL;

                _field = mysql_fetch_field( res_ );
                while ( NULL != _field )
                {
                    size_t    _sz  = 0;

                    switch ( _field->type )
                    {
                    case MYSQL_TYPE_TINY:
                        _sz = 1;
                        break;
                    case MYSQL_TYPE_SHORT:
                        _sz = 2;
                        break;
                    case MYSQL_TYPE_INT24:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONG:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONGLONG:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_FLOAT:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_DOUBLE:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_BLOB:
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                        _sz = _field->max_length;
                        break;
                    }

                    result_t *_res          = &_result[_idx];
                    _res->buf.assign( '\0', _sz );
                    _res->size              = _sz;
                    _res->type              = _field->type;
                    _res->is_null           = 0;

                    _bind[_idx].buffer_type   = _field->type;
                    _bind[_idx].buffer        = (void*)_res->buf.c_str();
                    _bind[_idx].buffer_length = _res->size;
                    _bind[_idx].length        = &_res->size;
                    _bind[_idx].is_unsigned   = _field->flags & UNSIGNED_FLAG;
                    _bind[_idx].is_null       = &_res->is_null;

                    ++ _idx;
                    _field = mysql_fetch_field( res_ );
                }

                if ( 0 == mysql_stmt_bind_result( stmt_, _bind ) )
                {
                    int _val = mysql_stmt_fetch( stmt_ );
                    if ( 0 == _val || MYSQL_DATA_TRUNCATED == _val )
                    {
                        result_t *_src = NULL;
                        result_t *_dest = NULL;

                        for ( size_t _idx = 0; _idx < _count; ++ _idx )
                        {
                            _src           = &_result[_idx];
                            _dest          = &m_ret_[_idx];
                            _dest->buf     = _src->buf;
                            _dest->size    = _src->size;
                            _dest->type    = _src->type;
                            _dest->is_null = _src->is_null;
                        }
                    }
                    _ret   = true;
                }

                free( _bind, _count * sizeof(MYSQL_BIND) );
                _result.clear();
            }
        }
    }

    return _ret;
}
