#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

#define MAX_PATH_LEN	  	        (260)
#define MAX_NAME_LEN				(32)
#define CONST_BUFF_SIZE				(8192)

/***************************************
//以下是
***************************************/
#define UPDATE_FLAG_CHANGE			0
#define UPDATE_FLAG_CREATE			1

#define UPDATE_DEST_OTHER			1
#define UPDATE_DEST_SERVER			2
#define UPDATE_DEST_MYSELF			3

//以下是服务器
#define INVALIDE_SERVERID			(0xffffffff)

enum  UpdateTypeEnum
{
	UT_Unknow,
	UT_Update,
	UT_New,
	UT_Delete
};

enum ClientState
{
	LS_Connect,
	PS_Connect,
	WS_,
	GS_,
	DS_
};




////////////////////////////////////////////


#endif //__COMMON_DEFINE_H__
