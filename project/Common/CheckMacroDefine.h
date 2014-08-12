#ifndef __CHECK_MACRO_DEFINE_H__
#define __CHECK_MACRO_DEFINE_H__

#include <assert.h>

#define ASSERT_FAIELD (assert(false))
#define ASSERT(x)     (assert(x))

#define CHECK_AND_RETURN(P, RET) \
	if(P == NULL)\
	{\
		return RET;	\
	}

#define CHECK_AND_RETURN_A(P, RET) \
	if(P == NULL)\
	{\
		ASSERT_FAIELD;\
		return RET;	\
	}

#define CHECK_RETURN_NULL(x)  CHECK_AND_RETURN(x, NULL)
#define CHECK_RETURN_TRUE(x)  CHECK_AND_RETURN(x, TRUE)
#define CHECK_RETURN_FALSE(x) CHECK_AND_RETURN(x, FALSE)

#define CHECK_RETURN_NULL_A(x)  CHECK_AND_RETURN_A(x, NULL)
#define CHECK_RETURN_TRUE_A(x)  CHECK_AND_RETURN_A(x, true)
#define CHECK_RETURN_FALSE_A(x) CHECK_AND_RETURN_A(x, false)

////////////////////////////////////////////


#endif //__CHECK_MACRO_DEFINE_H__
