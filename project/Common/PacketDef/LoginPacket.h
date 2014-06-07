#ifndef __LOGIN_PACKET__
#define __LOGIN_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StCharLoginReq	//登录请求消息
{
	CHAR szAccountName[32];	//账号名
	CHAR szPassword[32];	//密码
};

struct StCharLoginAck	//登录请求消息
{
	UINT8	byteCode;
	UINT64	u64CharID;
};



#pragma  pack(pop)



#endif /* __LOGIN_PACKET__ */
