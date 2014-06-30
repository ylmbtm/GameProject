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


enum  UpdateTypeEnum
{
	Update_Unknow,
	Update_Update,
	Update_New,
	Update_Delete
};






////////////////////////////////////////////


#endif //__COMMON_DEFINE_H__
