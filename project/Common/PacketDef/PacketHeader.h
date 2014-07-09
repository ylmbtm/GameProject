#ifndef __PACKET__HEADER_H__
#define __PACKET__HEADER_H__

#pragma  pack(push)
#pragma  pack(1)

struct TransferHeader
{
	UINT8	bCheckCode;
	UINT16  dwSize;
};

struct CommandHeader
{
	UINT8    CmdHandleID;
	UINT16   wCommandID;
	UINT32   dwSceneID;
	UINT64   u64CharID;		//角色ID
};

#pragma  pack(pop)



#endif /* __PACKET__HEADER_H__*/
