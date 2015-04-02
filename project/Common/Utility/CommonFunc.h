#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

#define GET_BIT(X,Y) (((X) >> (Y-1)) & 1)
#define SET_BIT(X,Y) ((X) |= (1 << (Y)))
#define CLR_BIT(X,Y) ((X) &= (~(1<<Y)))

namespace CommonFunc
{
	UINT32			GetProcessorNum();

	std::string		GetCurrentDir();

	BOOL			CreateDir(std::string &strDir);

	UINT32			GetCurrTime();

	UINT32			GetCurrDate();

	UINT32			GetTickCount();

	UINT64			GetTickCount64();

	UINT32			GetCurThreadID();

	UINT32			GetFreePhysMemory();  
}


#endif /* __COMMON_FUNCTION__*/
