#ifndef __SERVER_REPORT_PACKET__
#define __SERVER_REPORT_PACKET__

#pragma  pack(push)
#pragma  pack(1)

enum SvrReportID
{
	SVR_REPROT_NORMAL,		//基本日志

};

struct  StReportBase
{
	UINT32 dwReportID;
	UINT32 dwSvrID;
	UINT32 dwSvrType;
};

struct StNormalReport : public StReportBase
{
	CHAR szLogContent[256];
};


#pragma  pack(pop)



#endif /* __SERVER_REPORT_PACKET__ */
