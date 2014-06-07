#include "stdafx.h"
#include "IniFile.h"


CIniFile::CIniFile( void )
{

}

CIniFile::~CIniFile( void )
{

}

BOOL CIniFile::Load( std::string strFileName )
{
	FILE *pFile = fopen(strFileName.c_str(),"r+");

	if(pFile == NULL)
	{
		return FALSE;
	}

	CHAR szBuff[256] = {0};


	do
	{
		fgets(szBuff, 256, pFile);

		if(szBuff[0] == ';')
		{
			continue;
		}
		
		CHAR *pChar = strchr(szBuff,'=');
		if(pChar == NULL)
		{
			continue;
		}

		std::string strName;
		strName.assign(szBuff,pChar-szBuff);
		std::string strValue = pChar+1;

		strValue = strValue.substr(0,strValue.find_last_not_of(" \n\r\t")+1); 

		m_Values.insert(std::make_pair(strName, strValue));

	}while(!feof(pFile));
	
	fclose(pFile);


	return TRUE;
}
