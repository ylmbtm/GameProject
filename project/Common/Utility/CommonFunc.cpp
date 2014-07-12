#include "stdafx.h"
#include "CommonFunc.h"


UINT32 CommonFunc::GetProcessorNum()
{
	UINT32 dwNum = 0;
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	dwNum = sysInfo.dwNumberOfProcessors*2;
#else
	dwNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

	return dwNum;
}

std::string CommonFunc::GetCurrentDir()
{
	char szPath[MAX_PATH_LEN];

#ifdef WIN32
	_getcwd(szPath, MAX_PATH_LEN);
#else
	getcwd(szPath, MAX_PATH_LEN);
#endif
	return std::string(szPath);
}

UINT32 CommonFunc::GetTime()
{
	time_t t;    

	t=time(0);

	return (UINT32)t;
}


UINT32 CommonFunc::GetTickCount()
{
#ifdef WIN32
	return ::GetTickCount();
#else

	UINT32 dwTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC,&on) )
	{
		dwTickCount = on.tv_sec*1000+ on.tv_nsec/1000000;
	}

	return dwTickCount;

#endif
}

UINT64 CommonFunc::GetTickCount64()
{
#if WINVER < 0x0501
	return GetTickCount();
#else
#ifdef WIN32
		return ::GetTickCount64();
#else
	UINT64 dwTickCount = 0;;
	struct timespec on;
	if(0 == clock_gettime(CLOCK_MONOTONIC,&on) )
	{
		dwTickCount = on.tv_sec*1000+ on.tv_nsec/1000000;
	}

	return dwTickCount;
#endif
#endif
}

BOOL CommonFunc::CreateDir( std::string &strDir )
{
	int nRet = 0;
#ifdef WIN32
	nRet = _mkdir(strDir.c_str());
#else
	nRet = mkdir(strDir.c_str(), S_IRWXU);
#endif

	if(nRet == 0)
	{
		return TRUE;
	}

	if(errno == EEXIST)
	{
		return TRUE;
	}

	return FALSE;
}

UINT32 CommonFunc::GetCurThreadID()
{
	UINT32 dwThreadID = 0;
#ifdef WIN32
	dwThreadID = ::GetCurrentThreadId();
#else
	dwThreadID = (UINT32)pthread_self();
#endif
	return dwThreadID;
}

UINT32 CommonFunc::GetFreePhysMemory()
{
	UINT32 dwFreeSize = 0;
#ifdef WIN32
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx (&statex);

	dwFreeSize = (UINT32)(statex.ullAvailPhys/1024/1024);
#else
	UINT32 dwPageSize;
	UINT32 dwFreePages;
	dwPageSize = sysconf (_SC_PAGESIZE)/1024;
	dwFreePages = sysconf (_SC_AVPHYS_PAGES)/1024;
	dwFreeSize = dwFreePages * dwPageSize;
#endif

	return dwFreeSize;
}

