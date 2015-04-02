#include "stdafx.h"
#include "PropertyMgr.h"



BOOL CPropertyMgr::IsChanged()
{
	return TRUE;
}

BOOL CPropertyMgr::ClearChangeFlag()
{

	return TRUE;
}


UINT32 CPropertyMgr::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{

	return 0;
}

UINT32 CPropertyMgr::ReadFromBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag )
{
	return 0;
}

FLOAT CPropertyMgr::GetPropertyValue( UINT32 dwPropertyID )
{
	return m_PropertyList[dwPropertyID].GetCurValue();
}

VOID CPropertyMgr::SetProPertyValue( UINT32 dwPropertyID, FLOAT fValue )
{
	m_PropertyList[dwPropertyID].SetCurValue(fValue);

	return ;
}
