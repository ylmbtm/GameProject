#ifndef __CLIENT_PACKET__
#define __CLIENT_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StCharVerifyVersionReq
{
	UINT32 dwClientVersion;
};

struct StCharVerifyVersionAck
{
	UINT32 dwVerifyCode;
};

struct StCharNewAccountReq
{
	CHAR szAccountName[32];		//账号名
	CHAR szPassword[32];		//密码
};

struct StCharNewAccountAck
{
	UINT32 dwRetCode;
};

struct StCharNewCharReq
{
	UINT32 dwAccountID;	//账号ID
	UINT32 dwFeature;	// sex, face type, hair color, face color, and job
};

struct StCharNewCharAck
{
	UINT8  RetCode;
	UINT32 dwFeature;
	//need to be implement
};

struct StCharLoginReq	//登录请求消息
{
	CHAR szAccountName[32];	//账号名
	CHAR szPassword[32];	//密码
};

struct StCharPickInfo
{

};

struct StCharLoginAck	//登录请求消息
{
	UINT8	byteCode;
	UINT8   nCount;
	StCharPickInfo CharPickInfo[4];
	//StCharPickInfo;
	//need to be implement
	// this will return all the char belong to this char;
};


struct StCharPickCharReq	//选择角色请求
{
	UINT64  u64CharID;
};


struct StCharPickCharAck	//选择角色请求回复
{
	UINT8	byteCode;
	UINT64  u64CharID;
};




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



#endif /* __CLIENT_PACKET__ */
