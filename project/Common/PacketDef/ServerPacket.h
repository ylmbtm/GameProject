#ifndef __SERVER_PACKET__
#define __SERVER_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StSvrServerInfo
{
	UINT32	dwSvrID;	//服务器ID
	UINT8   dwType;	//服务器或玩家的类型
	UINT16  sPort;
	CHAR	szIpAddr[32];
};

struct StCharWillEnterGame
{
	UINT64  u64CharID;
	UINT32  dwIdentifyCode;
};

struct StSvrCreateSceneReq
{
	UINT32 dwSceneID;
	UINT32 CreateParam;
};

struct StSvrCreateSceneAck
{
	UINT32 dwAckCode;
	UINT32 dwServerID;
	UINT32 dwSceneID;
	UINT32 dwCreateParam;
};

struct StSvrEnterSceneReq
{
	UINT32 dwGameSvrID;
	UINT32 dwSceneID;
	UINT64 u64CharID;
	UINT32 dwProxySvrID;
};
 

#pragma  pack(pop)



#endif /* __SERVER_PACKET__ */
