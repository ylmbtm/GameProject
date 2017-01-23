#include "stdafx.h"
#include "GlobalConfig.h"
#include "Utility/CommonFunc.h"

CGlobalConfig::CGlobalConfig(void)
{
}

CGlobalConfig::~CGlobalConfig(void)
{
}

BOOL CGlobalConfig::InitVariant()
{
	m_strIpAddr		= m_IniFile.GetStringValue("ServerIP");
	m_dwServerID	= m_IniFile.GetIntValue("ServerID");
	m_sPort			= m_IniFile.GetIntValue("ServerPort");
	m_dwServerType	= m_IniFile.GetIntValue("ServerType");
	m_strCenterSvrIp = m_IniFile.GetStringValue("CenterSvrIP");
	m_sCenterSvrPort = m_IniFile.GetIntValue("CenterSvrPort");
	m_nMaxConnNum      = m_IniFile.GetIntValue("MaxConnNum");
	return TRUE;
}

BOOL CGlobalConfig::LoadFile( std::string strFileName )
{
	std::string strPath = CommonFunc::GetCurrentDir();

	strPath += ("/");

	return m_IniFile.Load(strPath + strFileName);
}

CGlobalConfig* CGlobalConfig::GetInstancePtr()
{
	static CGlobalConfig GlobalConfig;

	return &GlobalConfig;
}

BOOL CGlobalConfig::Load( std::string strFileName )
{
	if(!LoadFile(strFileName))
	{
		return FALSE;
	}

	if(!InitVariant())
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	return TRUE;
}
