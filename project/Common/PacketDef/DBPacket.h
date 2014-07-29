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

struct StDBDelCharReq
{
	UINT64 u64ConnID;

	StCharDelCharReq CharDelCharReq;
};

struct StDBCharDelCharAck
{
	UINT64 u64ConnID;

	StCharDelCharAck CharDelCharAck;
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

struct StDBLoadCharInfoReq
{
	UINT64  dwProxySvrID;	//角色对应的代理服务器ID
	UINT64  u64CharID;
};


struct StDBLoadCharInfoAck 
{
	UINT64 dwProxySvrID;
};



#pragma  pack(pop)



#endif /* __DB_PACKET__ */
