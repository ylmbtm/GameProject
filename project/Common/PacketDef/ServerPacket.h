#ifndef __SERVER_PACKET__
#define __SERVER_PACKET__

#pragma  pack(push)
#pragma  pack(1)

struct StRegisterToCenterSvr
{
	UINT32 dwSvrID;	//服务器ID或玩家ID
	UINT8  dwType;	//服务器或玩家的类型
	UINT16 sPort;
	CHAR  szIpAddr[32];
};

struct StActiveSvrList
{
	UINT32 dwCount;
	StRegisterToCenterSvr SvrInfo[10];
};

 

#pragma  pack(pop)



#endif /* __SERVER_PACKET__ */
