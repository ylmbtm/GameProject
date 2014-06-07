#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

//以下是对象类型
#define OBJECT_UNKNOW			0
#define OBJECT_PLAYER			1
#define OBJECT_NPC				2


//取对象类型
#define GET_OBJ_TYPE(x)			((UINT8)(x>>56))
//取玩家的用户ID 
#define GET_USER_ID(x)			((UINT32)x)
//合成玩家ID
#define MAKE_PLAYRE_ID(x)		(((UINT64)OBJECT_PLAYER)<<56|x)


/***************************************
//以下是
***************************************/
#define UPDATE_FLAG_CHANGE			0
#define UPDATE_FLAG_CREATE			1

#define UPDATE_DEST_OTHER			1
#define UPDATE_DEST_SERVER			2
#define UPDATE_DEST_MYSELF			3



////////////////////////////////////////////


#endif //__COMMON_DEFINE_H__
