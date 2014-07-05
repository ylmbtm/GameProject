
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
	bool SetFielsInfo(int nNum, ...);

	bool SetFieldNum(int nNum);

	int GetFieldNum();

	void SetFieldType(int nIndex, enum_field_types fdType);

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

	unsigned long nLen;


private:
    size_t                       m_RowCount;
	MYSQL_STMT					*m_pMySqlStmt;
	MYSQL_RES					*m_pResult;

	int							 m_nFieldNum;
	MYSQL_BIND					*m_pBinds;
};

#endif // _SQL_RESULT_H_