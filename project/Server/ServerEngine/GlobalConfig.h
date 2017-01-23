#ifndef _GLOBAL_CONFIG_H_
#define _GLOBAL_CONFIG_H_

#include "Utility/IniFile/IniFile.h"

class CGlobalConfig
{
private:
	CGlobalConfig(void);
	~CGlobalConfig(void);

public:
	static CGlobalConfig* GetInstancePtr();


	BOOL Load(std::string strFileName);



	UINT16		m_sPort;
	UINT32		m_dwServerID;
	UINT32		m_dwServerType;
	std::string m_strIpAddr;

	std::string m_strCenterSvrIp;
	UINT16		m_sCenterSvrPort;
	UINT32      m_nMaxConnNum;       //服务器支持的最大连接数


private:

	BOOL LoadFile(std::string strFileName);

	BOOL InitVariant();

	
private:
	CIniFile	m_IniFile;
};

#endif
