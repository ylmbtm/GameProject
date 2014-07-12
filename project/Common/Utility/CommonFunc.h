#ifndef __COMMON_FUNCTION__
#define __COMMON_FUNCTION__

namespace CommonFunc
{
	UINT32			GetProcessorNum();

	std::string		GetCurrentDir();

	BOOL			CreateDir(std::string &strDir);

	UINT32			GetTime();

	UINT32			GetTickCount();

	UINT64			GetTickCount64();

	UINT32			GetCurThreadID();

	UINT32			GetFreePhysMemory();  
}


#endif /* __COMMON_FUNCTION__*/
