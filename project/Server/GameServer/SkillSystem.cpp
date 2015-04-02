#include "stdafx.h"
#include "SkillSystem.h"



BOOL CSkillSystem::IsChanged()
{
	return TRUE;
}

BOOL CSkillSystem::ClearChangeFlag()
{

	return TRUE;
}


UINT32 CSkillSystem::WriteToBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest )
{

	return 0;
}

UINT32 CSkillSystem::ReadFromBuffer( CBufferHelper *pBufHelper, UINT32 dwChangeFlag )
{
	return 0;
}
