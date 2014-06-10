#ifndef __DB_PACKET__
#define __DB_PACKET__
#include "ClientPacket.h"

#pragma  pack(push)
#pragma  pack(1)


struct StDBNewAccountReq
{
	UINT64 u64ConnID;

	StCharNewAccountReq CharNewAccountReq;
};

struct StDBNewAccountAck
{
	UINT64 u64ConnID;

	StCharNewAccountAck CharNewAccountAck;
};

struct StDBNewCharReq
{
	UINT64 u64ConnID;

	StCharNewCharReq CharNewCharReq;
};

struct StDBCharNewCharAck
{
	UINT64 u64ConnID;

	StCharNewCharAck CharNewCharAck;
};

struct StDBCharLoginReq	//登录请求消息
{
	UINT64 u64ConnID;

	StCharLoginReq CharLoginReq;
};

struct StDBCharLoginAck	//登录请求消息
{
	UINT64 u64ConnID;

	StCharLoginAck CharLoginAck;
};


struct StDBCharPickCharReq	//选择角色请求
{
	UINT64 u64ConnID;

	StCharPickCharReq CharPickCharReq;
};


struct StDBCharPickCharAck	//选择角色请求回复
{
	UINT64 u64ConnID;

	StCharPickCharAck CharPickCharAck;
};




#pragma  pack(pop)



#endif /* __DB_PACKET__ */
