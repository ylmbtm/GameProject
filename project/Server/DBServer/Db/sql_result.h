
#ifndef _SQL_RESULT_H_
#define _SQL_RESULT_H_

#include <my_global.h>
#include <mysql.h>
#include <utility>
#include "sql_string.h"
#include "Allocator.h"
using namespace std;

namespace ex
{

class sql_result
{
public:
    friend class sql_conn;
    friend object_pool<sql_result>;

    // close.
    void release( void );

    // next.
    bool next( void );

    // count.
    size_t count( void ) const;

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
    std::pair<size_t, char const*> get_string( size_t idx_ );

    // get blob.
    std::pair<size_t, void const*> get_blob( size_t idx_ );

	// get medium blob.
	std::pair<size_t, void const*> get_medium_blob( size_t idx_ );

    // get returned bool value of out parameter.
    my_bool getr_bool( size_t idx_ );

    // get returned int8 value of out parameter.
    int8 getr_int8( size_t idx_ );

    // get returned uint8 value of out parameter.
    uint8 getr_uint8( size_t idx_ );

    // get returned int16 value of out parameter.
    int16 getr_int16( size_t idx_ );

    // get returned uint16 value of out parameter.
    uint16 getr_uint16( size_t idx_ );

    // get returned int32 value of out parameter.
    int32 getr_int32( size_t idx_ );

    // get returned uint32 value of out parameter.
    uint32 getr_uint32( size_t idx_ );

    // get returned int64 value of out parameter.
    int64 getr_int64( size_t idx_ );

    // get returned uint64 value of out parameter.
    uint64 getr_uint64( size_t idx_ );

    // get returned float value of out parameter.
    float getr_float( size_t idx_ );

    // get returned double value of out parameter.
    double getr_double( size_t idx_ );

    // get returned string of out parameter.
    std::pair<size_t, char const*> getr_string( size_t idx_ );

    // get returned blob of out parameter.
    std::pair<size_t, void const*> getr_blob( size_t idx_ );

protected:
    sql_result( void );
    ~sql_result( void );

    typedef struct _result
    {
        sql_string       buf;
        unsigned long    size;
        enum_field_types type;
        my_bool          is_null;

        _result( void )
            : size( 0 ),
              type( MYSQL_TYPE_NULL ),
              is_null( 0 )
        {}
    } result_t;

    // initialize.
    bool _init( MYSQL_STMT *stmt_, MYSQL_RES *res_ );

    // returned output parameters.
    bool _retval( MYSQL_STMT *stmt_, MYSQL_RES *res_ );

private:
    list<map<size_t, _result> >           m_res_; // results.
    list<map<size_t, _result> >::iterator m_cur_; // current result.
    size_t                                m_row_;
    map<size_t, _result>                  m_ret_; // out parameters.
};

} // namespace ex

#endif // _SQL_RESULT_H_