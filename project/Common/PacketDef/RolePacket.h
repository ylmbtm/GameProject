#ifndef __ROLE_PACKET__
#define __ROLE_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StCharEnterGameReq
{
	UINT64 u64CharID;		//进入游戏请求
	UINT32 dwEnterCode;		//进入游戏请求码
};

struct StCharEnterGameAck
{
	UINT32 dwEnterCode;		//进入游戏请求码
};

struct StCharLeaveGameReq
{
	UINT32 dwLeaveReason;
};

struct StCharLeaveGameAck
{

};


struct StCharMoveReq
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
};

#pragma  pack(pop)



#endif /* __TRANSFER_PACKET__ */
