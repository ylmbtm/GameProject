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
	m_RowCount		= 0;
	m_pMySqlStmt	= 0;
	m_pResult		= 0;
	m_nFieldNum		= 0;
	m_pBinds		= 0;
}

CDBRecordSet::~CDBRecordSet( void )
{

}

bool CDBRecordSet::MoveNext( void )
{
	//获取数据成功
	if(0 == mysql_stmt_fetch(m_pMySqlStmt))
	{
		return true;
	}

    return false;
}

size_t CDBRecordSet::GetCount( void )
{
	if(m_pMySqlStmt == NULL)
	{
		ASSERT_FAIELD;
		return 0;
	}

	m_RowCount  = (size_t)mysql_stmt_num_rows(m_pMySqlStmt);

	return m_RowCount;
}

my_bool CDBRecordSet::get_bool( size_t idx_ )
{
	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

	my_bool Ret = false;
	switch (pTemp->buffer_type)
	{
	case MYSQL_TYPE_TINY:
		Ret = ( 0 != ( *(uint8*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_SHORT:
		Ret = ( 0 != ( *(uint16*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_INT24:
		Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_LONG:
		Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_LONGLONG:
		Ret = ( 0 != ( *(uint64*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_FLOAT:
		Ret = ( 0 != ( *(uint32*)pTemp->buffer ) );
		break;
	case MYSQL_TYPE_DOUBLE:
		Ret = ( 0 != ( *(uint64*)pTemp->buffer ) );
		break;
	}

    return Ret;
}

int8 CDBRecordSet::get_int8( size_t idx_ )
{
	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

	int8 Ret = 0;
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        Ret = (int8)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        Ret = (int8)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        Ret = (int8)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        Ret = (int8)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        Ret = (int8)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        Ret = (int8)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        Ret = (int8)( *(double*)pTemp->buffer );
        break;
    }

    return Ret;
}

uint8 CDBRecordSet::get_uint8( size_t idx_ )
{
    uint8 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (uint8)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (uint8)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (uint8)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (uint8)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (uint8)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (uint8)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (uint8)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

int16 CDBRecordSet::get_int16( size_t idx_ )
{
    int16 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (int16)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (int16)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (int16)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (int16)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (int16)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (int16)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (int16)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

uint16 CDBRecordSet::get_uint16( size_t idx_ )
{
    uint16 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (uint16)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (uint16)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (uint16)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (uint16)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (uint16)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (uint16)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (uint16)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

int32 CDBRecordSet::get_int32( size_t idx_ )
{
    int32 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (int32)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (int32)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (int32)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (int32)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (int32)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (int32)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (int32)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

uint32 CDBRecordSet::get_uint32( size_t idx_ )
{
    uint32 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (uint32)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (uint32)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (uint32)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (uint32)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (uint32)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (uint32)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (uint32)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

int64 CDBRecordSet::get_int64( size_t idx_ )
{
    int64 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (int64)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (int64)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (int64)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (int64)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (int64)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (int64)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (int64)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

uint64 CDBRecordSet::get_uint64( size_t idx_ )
{
    uint64 _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (uint64)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (uint64)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (uint64)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (uint64)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (uint64)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (uint64)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (uint64)( *(double*)pTemp->buffer );
        break;
    }
    return _ret;
}

float CDBRecordSet::get_float( size_t idx_ )
{
    float _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (float)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (float)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (float)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (float)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (float)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (float)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (float)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

double CDBRecordSet::get_double( size_t idx_ )
{
    double _ret = 0;

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}
    switch ( pTemp->buffer_type )
    {
    case MYSQL_TYPE_TINY:
        _ret = (double)( *(uint8*)pTemp->buffer );
        break;
    case MYSQL_TYPE_SHORT:
        _ret = (double)( *(uint16*)pTemp->buffer );
        break;
    case MYSQL_TYPE_INT24:
        _ret = (double)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONG:
        _ret = (double)( *(uint32*)pTemp->buffer );
        break;
    case MYSQL_TYPE_LONGLONG:
        _ret = (double)( *(uint64*)pTemp->buffer );
        break;
    case MYSQL_TYPE_FLOAT:
        _ret = (double)( *(float*)pTemp->buffer );
        break;
    case MYSQL_TYPE_DOUBLE:
        _ret = (double)( *(double*)pTemp->buffer );
        break;
    }

    return _ret;
}

char* CDBRecordSet::get_string( size_t idx_ )
{
	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return false;
	}

	return (char*)pTemp->buffer;
}

std::pair<size_t, void const*> CDBRecordSet::get_blob( size_t idx_ )
{
    std::pair<size_t, void const*> _ret( 0, "" );

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return _ret;
	}

	_ret.first		= pTemp->length_value;
	_ret.second	= pTemp->buffer;

    return _ret;
}

// get medium blob.
std::pair<size_t, void const*> CDBRecordSet::get_medium_blob( size_t idx_ )
{
	std::pair<size_t, void const*> _ret( 0, "" );

	MYSQL_BIND *pTemp = &m_pBinds[idx_];

	if(pTemp->is_null_value)
	{
		return _ret;
	}

	_ret.first		= pTemp->length_value;
	_ret.second		= pTemp->buffer;

	return _ret;
}

bool CDBRecordSet::InitRecordSet(MYSQL_STMT *pMySqlStmt, MYSQL_RES *pResult)
{
	if((pMySqlStmt == NULL)||(pResult == NULL))
	{
		ASSERT_FAIELD;
		return false;
	}

	if (0 != mysql_stmt_bind_result(pMySqlStmt, m_pBinds))
	{
		mysql_stmt_close( pMySqlStmt );

		pMySqlStmt = NULL;

		ASSERT_FAIELD;
		return false;
	}
	
	m_pMySqlStmt = pMySqlStmt;
	m_pResult    = pResult;

    return true;
}

bool CDBRecordSet::SetFielsInfo( int nNum, ... )
{
	va_list argList;
	va_start( argList, nNum );

	SetFieldNum(nNum);

	for (int i=0; i < nNum; i++)
	{
		enum_field_types fdType = va_arg(argList, enum_field_types);
		SetFieldType(i, fdType);
	}

	va_end( argList );

	return true;
}

bool CDBRecordSet::SetFieldNum( int nNum )
{
	if(nNum <= 0)
	{
		ASSERT_FAIELD;
		return false;
	}

	m_nFieldNum = nNum;

	m_pBinds = new MYSQL_BIND[nNum];

	memset(m_pBinds, 0, sizeof(MYSQL_BIND)*nNum);

	return true;
}

int CDBRecordSet::GetFieldNum()
{
	return m_nFieldNum;
}

void CDBRecordSet::SetFieldType( int nIndex, enum_field_types fdType )
{
	MYSQL_BIND *pTemp = &m_pBinds[nIndex];

	int BufferSize = 0;

	switch ( fdType )
	{
	case MYSQL_TYPE_TINY:		BufferSize = 1;	break;
	case MYSQL_TYPE_SHORT:		BufferSize = 2;	break;
	case MYSQL_TYPE_INT24:		BufferSize = 4;	break;
	case MYSQL_TYPE_LONG:		BufferSize = 4;	break;
	case MYSQL_TYPE_LONGLONG:	BufferSize = 8;break;
	case MYSQL_TYPE_FLOAT:		BufferSize = 4;	break;
	case MYSQL_TYPE_DOUBLE:		BufferSize = 8;	break;
	case MYSQL_TYPE_STRING:		BufferSize = 255;break;
	case MYSQL_TYPE_VAR_STRING:	BufferSize = 65535;break;
	case MYSQL_TYPE_TINY_BLOB:	BufferSize = 256;break;
	case MYSQL_TYPE_BLOB:		BufferSize = 65535;break;
	case MYSQL_TYPE_MEDIUM_BLOB:BufferSize = 16777215;break;
	case MYSQL_TYPE_LONG_BLOB:	BufferSize = 4026531840;break;
	}

	nLen = 0;
	pTemp->buffer			= malloc(BufferSize);
	pTemp->buffer_length	= BufferSize;
	pTemp->buffer_type      = fdType;
	pTemp->length			= &nLen;

	return ;
}
