#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

//角色状态
typedef struct St_ObjectStatus
{
	union
	{
		UINT32  dwValues;
		struct
		{
			UINT32	nDir:9;			// 朝向
			UINT32	bDead:1;		// 是否死亡
		};
	};
}St_ObjectStatus;


////////////////////////////////////////////


#endif //__GAME_STRUCT_H__
