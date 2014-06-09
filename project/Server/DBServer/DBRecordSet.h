
#ifndef _SQL_RESULT_H_
#define _SQL_RESULT_H_

#include <my_global.h>
#include <mysql.h>

class CDBRecordSet
{
public:
	CDBRecordSet( void );
	~CDBRecordSet( void );

public:
	bool SetFielsInfo(int nNum, ...)
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

	bool SetFieldNum(int nNum)
	{
		m_nFieldNum = nNum;
		if(nNum > 0)
		{
			m_pBinds = new MYSQL_BIND[nNum];

			memset(m_pBinds, 0, sizeof(MYSQL_BIND)*nNum);
		}

		return true;
	}

	void SetFieldType(int nIndex, enum_field_types fdType)
	{
		MYSQL_BIND *pTemp = &m_pBinds[nIndex];

		int BufferSize = 0;

		switch ( fdType )
		{
		case MYSQL_TYPE_TINY:	BufferSize = 1;	break;
		case MYSQL_TYPE_SHORT:	BufferSize = 2;	break;
		case MYSQL_TYPE_INT24:	BufferSize = 4;	break;
		case MYSQL_TYPE_LONG:	BufferSize = 4;	break;
		case MYSQL_TYPE_LONGLONG:BufferSize = 8;break;
		case MYSQL_TYPE_FLOAT:	BufferSize = 4;	break;
		case MYSQL_TYPE_DOUBLE:	BufferSize = 8;	break;
		case MYSQL_TYPE_STRING: BufferSize = 255;break;
		case MYSQL_TYPE_VAR_STRING:BufferSize = 65535;break;
		case MYSQL_TYPE_BLOB:	BufferSize = 256;break;
		case MYSQL_TYPE_TINY_BLOB:BufferSize = 65535;break;
		case MYSQL_TYPE_MEDIUM_BLOB:BufferSize = 16777215;break;
		case MYSQL_TYPE_LONG_BLOB:BufferSize = 4026531840;break;
		}

		pTemp->buffer			= malloc(BufferSize);
		pTemp->buffer_length	= BufferSize;
		pTemp->buffer_type      = fdType;
	}

public:
    // next.
    bool MoveNext( void );

    // count.
    size_t GetCount( void );

    // get bool value.
    my_bool get_bool( size_t idx_ );

    // get int8 value.
    int8 get_int8( size_t idx_ );

    // get uint8 value.
    uint8 get_uint8( size_t idx_ );

    // get int16 value.
    int16 get_int16( size_t idx_ );

    // get uint16 value.
    uint16 get_uint16( size_t idx_ );

    // get int32 value.
    int32 get_int32( size_t idx_ );

    // get uint32 value.
    uint32 get_uint32( size_t idx_ );

    // get int64 value.
    int64 get_int64( size_t idx_ );

    // get uint64 value.
    uint64 get_uint64( size_t idx_ );

    // get float value.
    float get_float( size_t idx_ );

    // get double value.
    double get_double( size_t idx_ );

    // get string.
    char* get_string( size_t idx_ );

    // get blob.
    std::pair<size_t, void const*> get_blob( size_t idx_ );

	// get medium blob.
	std::pair<size_t, void const*> get_medium_blob( size_t idx_ );

    // initialize.
    bool InitRecordSet(MYSQL_STMT *pMySqlStmt, MYSQL_RES *pResult);


private:
    size_t                       m_RowCount;
	MYSQL_STMT					*m_pMySqlStmt;
	MYSQL_RES					*m_pResult;


	int							 m_nFieldNum;
	MYSQL_BIND					*m_pBinds;
};

#endif // _SQL_RESULT_H_