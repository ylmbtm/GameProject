#ifndef __SKILL_SYSTEM_H__
#define __SKILL_SYSTEM_H__


class CSkillSystem
{
public:
	CSkillSystem();

	~CSkillSystem();

public:
	virtual BOOL	IsChanged();

	virtual BOOL	ClearChangeFlag();

	virtual UINT32	WriteToBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag, UINT32 dwDest);

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper, UINT32 dwChangeFlag);

public:
	
};

#endif //__SKILL_SYSTEM_H__
