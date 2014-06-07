#ifndef _GLOBAL_LOG_H_
#define _GLOBAL_LOG_H_

#include "Utility/CritSec.h"

class CLog
{
private:
	CLog(void);
	~CLog(void);

public:
	static CLog* GetInstancePtr();

	BOOL StartLog(std::string strPrefix, std::string strLogDir = "");

	BOOL CloseLog();

	void AddLog(char* lpszFormat,...);

protected:
	CCritSec		  m_CritSec;

	INT32			  m_LogCount;

	FILE *m_pLogFile;
};

#endif