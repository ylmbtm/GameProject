#ifndef __TRANSFER_PACKET__
#define __TRANSFER_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StConnectNotify
{
	UINT8  btConType;	//服务器或玩家的类型
	UINT64 u64ConnID;	//服务器ID或玩家ID
};

struct StDisConnectNotify
{
	UINT8  btConType;	//服务器或玩家的类型
	UINT64 u64ConnID;	//服务器ID或玩家ID
};



#pragma  pack(pop)



#endif /* __TRANSFER_PACKET__ */
